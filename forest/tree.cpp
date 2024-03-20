#include <forest/tree.h>

#include <cassert>

// ################################################################# //
// ########################### Node BASE ########################### //
// ################################################################# //

Node::Node(Node* parent, Vec2f position, double angle, double length, const Phenotype::SPtr& phenotype)
    : _parent(parent)
    , _depth(parent ? parent->GetDepth() : 0)
    , _position(position)
    , _angle(angle)
    , _length(length)
    , _phenotype(phenotype)
{}

Vec2f Node::_GetEdge() const {
    Vec2f edge = _position + Vec2f{1, 0}.rotated(_angle) * _length;
    return edge;
}

size_t Node::GetDepth() const {
    return _depth;
}

double Node::_CollectFoodDfs() {
    double food = CollectFood();
    for (SPtr& child : _children) {
        food += child->_CollectFoodDfs();
    }
    return food;
}

void Node::_TickDfs(double food, double elapsedSec) {
    Tick(food, elapsedSec);

    assert(food > -ExtMath::EPS);

    if (_parent) {
        _position = _parent->_GetEdge();
    }

    double foodPart = food / _children.size();

    size_t childrenToTick = _children.size();

    for (size_t i = 0; i < childrenToTick; ++i) {
        _children[i]->_TickDfs(foodPart, elapsedSec);
    }
}

void Node::_RenderDfs(REngine::Graphics* gr) const {
    Render(gr);

    for (const SPtr& child : _children) {
        child->_RenderDfs(gr);
    }
}

void Node::AddChild(const SPtr& child) {
    _children.push_back(child);
}

// ################################################################# //
// ########################### Node SEED ########################### //
// ################################################################# //

NodeSeed::NodeSeed(Vec2f position, const Phenotype::SPtr& phenotype)
    : Node(nullptr, position, 0, 0, phenotype)
    , _branchBase(new NodeBranch(this, position, ExtMath::PI * -0.5, phenotype))
    , _rootBase(new NodeRoot(this, position, ExtMath::PI * 0.5, phenotype))
{
    _foodStorage = 100000;
}

double NodeSeed::CollectFood() {
    return 0;
}

void NodeSeed::Tick(double&/* food*/, double elapsedSec) {
    double food = 0;
    if (_foodStorage > 0) {
        food += std::min(_foodStorage,
                _phenotype->AccessTraits(_depth).GetSeedFoodStorageConsumption() * elapsedSec);
        _foodStorage = std::max(0., _foodStorage - food);
    }
    food += _rootBase->CollectFood();
    food += _branchBase->CollectFood();

    double distribution = _phenotype->AccessTraits(_depth).GetSeedFoodDistribution();
    _rootBase->_TickDfs(food * distribution, elapsedSec);
    _branchBase->_TickDfs(food * (1 - distribution), elapsedSec);
}

// ################################################################# //
// ########################## Node SPROUT ########################## //
// ################################################################# //

NodeSprout::NodeSprout(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, 0, phenotype)
    , _active(true)
    , _foodAccumulated(0)
{}

double NodeSprout::CollectFood() {
    return 0;
}

void NodeSprout::Tick(double& food, double elapsedSec) {
    _foodAccumulated += food;
    food = 0;

    if (_active && _foodAccumulated > _phenotype->AccessTraits(_depth).GetSproutAccumulatedToGrow()) {
        _foodAccumulated = 0;
        double deviation = _phenotype->AccessTraits(_depth).GetSproutAngleDeviation();
        double deltaAngle = ExtMath::RandomDouble(-deviation, deviation);
        _parent->AddChild(std::make_shared<NodeBranch>(_parent, _position, _angle + deltaAngle, _phenotype));

        if (ExtMath::RandomDouble(0, 1) < _phenotype->AccessTraits(_depth).GetSproutChanceToTerminate()) {
            _active = false;
        }
    }
}

// ################################################################# //
// ########################## Node BRANCH ########################## //
// ################################################################# //

NodeBranch::NodeBranch(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, 10, phenotype)
{
    AddChild(std::make_shared<NodeSprout>(this, _GetEdge(), angle, _phenotype));
}

double NodeBranch::CollectFood() {
    return 0;
}

void NodeBranch::Tick(double& food, double elapsedSec) {
    double growthConsumption = food * _phenotype->AccessTraits(_depth).GetBranchFoodDistribution();
    food -= growthConsumption;
    _length += growthConsumption * _phenotype->AccessTraits(_depth).GetBranchGrowthSpeed() / _length;
}

// ################################################################# //
// ########################### Node ROOT ########################### //
// ################################################################# //

NodeRoot::NodeRoot(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, 10, phenotype)
{}

double NodeRoot::CollectFood() {
    return 0;
}

void NodeRoot::Tick(double& food, double elapsedSec) {
    food = 0;
}

