#include <forest/tree.h>

#include <cassert>

// ################################################################# //
// ########################### Node BASE ########################### //
// ################################################################# //

Node::Node(Node* parent, Vec2f position, float angle, float length, const Phenotype::SPtr& phenotype)
    : _parent(parent)
    , _position(position)
    , _angle(angle)
    , _length(length)
    , _phenotype(phenotype)
{}

Vec2f Node::_GetEdge() const {
    Vec2f edge = _position + Vec2f{1, 0}.rotated(_angle) * _length;
    return edge;
}

float Node::_CollectFoodDfs() {
    float food = CollectFood();
    for (SPtr& child : _children) {
        food += child->_CollectFoodDfs();
    }
    return food;
}

void Node::_TickDfs(float food, float elapsedSec) {
    Tick(food, elapsedSec);

    assert(food > -ExtMath::EPS);

    if (_parent) {
        _position = _parent->_GetEdge();
    }

    float foodPart = food / _children.size();

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
    _foodStorage = 10000;
}

float NodeSeed::CollectFood() {
    return 0;
}

void NodeSeed::Tick(float&/* food*/, float elapsedSec) {
    float food = 0;
    if (_foodStorage > 0) {
        food += std::min(_foodStorage, _phenotype->GetSeedFoodStorageConsumption() * elapsedSec);
        _foodStorage = std::max(0.f, _foodStorage - food);
    }
    food += _rootBase->CollectFood();
    food += _branchBase->CollectFood();

    float distribution = _phenotype->GetSeedFoodDistribution();
    _rootBase->_TickDfs(food * distribution, elapsedSec);
    _branchBase->_TickDfs(food * (1 - distribution), elapsedSec);
}

// ################################################################# //
// ########################## Node SPROUT ########################## //
// ################################################################# //

NodeSprout::NodeSprout(Node* parent, Vec2f position, float angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, 0, phenotype)
    , _active(true)
    , _foodAccumulated(0)
{}

float NodeSprout::CollectFood() {
    return 0;
}

void NodeSprout::Tick(float& food, float elapsedSec) {
    _foodAccumulated += food;
    food = 0;

    if (_active && _foodAccumulated > _phenotype->GetSproutAccumulatedToGrow()) {
        _foodAccumulated = 0;
        float deviation = _phenotype->GetSproutAngleDeviation();
        float deltaAngle = ExtMath::RandomDouble(-deviation, deviation);
        _parent->AddChild(std::make_shared<NodeBranch>(_parent, _position, _angle + deltaAngle, _phenotype));

        if (ExtMath::RandomDouble(0, 1) < _phenotype->GetSproutChanceToTerminate()) {
            _active = false;
        }
    }
}

// ################################################################# //
// ########################## Node BRANCH ########################## //
// ################################################################# //

NodeBranch::NodeBranch(Node* parent, Vec2f position, float angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, 10, phenotype)
{
    AddChild(std::make_shared<NodeSprout>(this, _GetEdge(), angle, _phenotype));
}

float NodeBranch::CollectFood() {
    return 0;
}

void NodeBranch::Tick(float& food, float elapsedSec) {
    float growthConsumption = food * _phenotype->GetBranchFoodDistribution();
    food -= growthConsumption;
    _length += growthConsumption * _phenotype->GetBranchGrowthCost();
}

// ################################################################# //
// ########################### Node ROOT ########################### //
// ################################################################# //

NodeRoot::NodeRoot(Node* parent, Vec2f position, float angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, 10, phenotype)
{}

float NodeRoot::CollectFood() {
    return 0;
}

void NodeRoot::Tick(float& food, float elapsedSec) {
    food = 0;
}

