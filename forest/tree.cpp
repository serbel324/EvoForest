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

        if (ExtMath::RandomDouble(0, 1) > _phenotype->AccessTraits(_depth).GetSproutChanceToSpawnLeaf()) {
            _SpawnBranch();
        } else {
            _SpawnLeaf();
        }

        if (ExtMath::RandomDouble(0, 1) < _phenotype->AccessTraits(_depth).GetSproutChanceToTerminate()) {
            _active = false;
        }
    }
}

void NodeSprout::_SpawnBranch() {
    double deviationMin = _phenotype->AccessTraits(_depth).GetSproutBranchAngleDeviationMin();
    double deviationRange = _phenotype->AccessTraits(_depth).GetSproutBranchAngleDeviationRange();
    int sign = ExtMath::RandomInt(0, 2) * 2 - 1;
    double deltaAngle = ExtMath::RandomDouble(deviationMin, deviationMin + deviationRange) * sign;
    _parent->AddChild(std::make_shared<NodeBranch>(_parent, _position, _angle + deltaAngle, _phenotype));
}

void NodeSprout::_SpawnLeaf() {
    double deviationMin = _phenotype->AccessTraits(_depth).GetSproutLeafAngleDeviationMin();
    double deviationRange = _phenotype->AccessTraits(_depth).GetSproutLeafAngleDeviationRange();
    int sign = ExtMath::RandomInt(0, 2) * 2 - 1;
    double deltaAngle = ExtMath::RandomDouble(deviationMin, deviationMin + deviationRange) * sign;
    _parent->AddChild(std::make_shared<NodeLeaf>(_parent, _position, _angle + deltaAngle, _phenotype));
}

// ################################################################# //
// ########################## Node BRANCH ########################## //
// ################################################################# //

NodeBranch::NodeBranch(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, phenotype->AccessTraits(parent->GetDepth() + 1).GetBranchInitialLength(), phenotype)
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
// ########################### Node LEAF ########################### //
// ################################################################# //

NodeLeaf::NodeLeaf(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle,
            phenotype->AccessTraits(parent->GetDepth() + 1).GetLeafStemLength(),
            phenotype)
    , _brightness(0)
{}

double NodeLeaf::CollectFood() {
    return _brightness * _phenotype->AccessTraits(_depth).GetLeafFoodGenerationSpeed();
}

void NodeLeaf::Tick(double& food, double elapsedSec) {
    food = 0;
}

void NodeLeaf::SetBrightness(double brightness) {
    _brightness = brightness;
}

// ################################################################# //
// ########################### Node ROOT ########################### //
// ################################################################# //

NodeRoot::NodeRoot(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, phenotype->AccessTraits(parent->GetDepth() + 1).GetRootInitialLength(), phenotype)
    , _growthStopped(false)
{
    AddChild(std::make_shared<NodeRootSprout>(this, _GetEdge(), angle, _phenotype));
}

double NodeRoot::CollectFood() {
    return 0;
}

void NodeRoot::Tick(double& food, double elapsedSec) {
    if (!_growthStopped) {
        double growthConsumption = food * _phenotype->AccessTraits(_depth).GetRootFoodDistribution();
        food -= growthConsumption;
        _length += growthConsumption * _phenotype->AccessTraits(_depth).GetRootGrowthSpeed() / _length;
    }
}

void NodeRoot::StopGrowth() {
    _growthStopped = true;
}

// ################################################################# //
// ####################### Node ROOT_SPROUT  ####################### //
// ################################################################# //

NodeRootSprout::NodeRootSprout(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, 0, phenotype)
    , _active(true)
    , _foodAccumulated(0)
{}

double NodeRootSprout::CollectFood() {
    return 0;
}

void NodeRootSprout::Tick(double& food, double elapsedSec) {
    _foodAccumulated += food;
    food = 0;

    if (_active && _foodAccumulated > _phenotype->AccessTraits(_depth).GetRootSproutAccumulatedToGrow()) {
        _foodAccumulated = 0;

        if (ExtMath::RandomDouble(0, 1) > _phenotype->AccessTraits(_depth).GetRootSproutChanceToSpawnMiner()) {
            _SpawnMiner();
        } else {
            _SpawnRoot();
        }

        dynamic_cast<NodeRoot*>(_parent)->StopGrowth();

        if (ExtMath::RandomDouble(0, 1) < _phenotype->AccessTraits(_depth).GetRootSproutChanceToTerminate()) {
            _active = false;
        }
    }
}

void NodeRootSprout::_SpawnRoot() {
    double deviationMin = _phenotype->AccessTraits(_depth).GetRootSproutAngleDeviationMin();
    double deviationRange = _phenotype->AccessTraits(_depth).GetRootSproutAngleDeviationRange();
    int sign = ExtMath::RandomInt(0, 2) * 2 - 1;
    double deltaAngle = ExtMath::RandomDouble(deviationMin, deviationMin + deviationRange) * sign;
    _parent->AddChild(std::make_shared<NodeRoot>(_parent, _position, _angle + deltaAngle, _phenotype));
}

void NodeRootSprout::_SpawnMiner() {
    double deviationMin = _phenotype->AccessTraits(_depth).GetRootSproutMinerAngleDeviationMin();
    double deviationRange = _phenotype->AccessTraits(_depth).GetRootSproutMinerAngleDeviationRange();
    int sign = ExtMath::RandomInt(0, 2) * 2 - 1;
    double deltaAngle = ExtMath::RandomDouble(deviationMin, deviationMin + deviationRange) * sign;
    _parent->AddChild(std::make_shared<NodeMiner>(_parent, _position, _angle + deltaAngle, _phenotype));
}

// ################################################################# //
// ########################## Node MINER  ########################## //
// ################################################################# //

NodeMiner::NodeMiner(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, phenotype->AccessTraits(parent->GetDepth() + 1).GetMinerStemLength(), phenotype)
    , _mineralConcentration(0)
{}

double NodeMiner::CollectFood() {
    return _mineralConcentration * _phenotype->AccessTraits(_depth).GetMinerFoodGenerationSpeed();
}

void NodeMiner::Tick(double& food, double elapsedSec) {
    food = 0;
}

void NodeMiner::SetMineralConcentration(double mineralConcentration) {
    _mineralConcentration = mineralConcentration;
}
