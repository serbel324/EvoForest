#include <forest/tree.h>

#include <cassert>
#include <sstream>

// TODO split into separate files

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

Vec2f Node::GetEdge() const {
    Vec2f edge = _position + Vec2f{1, 0}.rotated(_angle) * _length;
    return edge;
}

size_t Node::GetDepth() const {
    return _depth;
}

Vec2f Node::GetPosition() const {
    return _position;
}

double Node::GetSubtreeMaintenanceConsumption() const {
    return _subtreeMaintenanceConsumption;
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

void Node::_Update(World* /*world*/) {
    if (_parent) {
        _position = _parent->GetEdge();
    }

    _maintenanceConsumption = _GetMaintenanceConsumption();
    _subtreeMaintenanceConsumption = _maintenanceConsumption;
}

void Node::_UpdateDfs(World* world) {
    _Update(world);

    for (Node::SPtr& child : _children) {
        child->_UpdateDfs(world);
        _subtreeMaintenanceConsumption += child->GetSubtreeMaintenanceConsumption();
    }
}

const Phenotype::TraitAccessor& Node::_AccessTraits() const {
    return _phenotype->AccessTraits(_depth);
}

std::string Node::ToString() const {
    std::stringstream str;
    Print(str);
    return str.str();
}

void Node::Print(std::ostream& out) const {
    out << "Node {}";
}

void Node::PrintSubtree(std::ostream& out, const std::string& prefix) const {
    out << prefix;
    Print(out);
    out << " _position={" << _position << "}" << std::endl;
    if (_parent) {
        out << "^ ";
        _parent->Print(out);
        out << "edge={" << _parent->GetEdge() << "}" << std::endl;
    }
    for (const Node::SPtr& child : _children) {
        child->PrintSubtree(out, prefix + "-");
    }
}

// ################################################################# //
// ########################### Node SEED ########################### //
// ################################################################# //

NodeSeed::NodeSeed(Vec2f position, const Phenotype::SPtr& phenotype)
    : Node(nullptr, position, 0, 0, phenotype)
    , _branchBase(new NodeBranch(this, position, ExtMath::PI * -0.5, phenotype))
    , _rootBase(new NodeRoot(this, position, ExtMath::PI * 0.5, phenotype))
{
    AddChild(_branchBase);
    AddChild(_rootBase);
    _foodStorage = 1000;
}

double NodeSeed::CollectFood() {
    return 0;
}

void NodeSeed::Tick(double&/* food*/, double elapsedSec) {
    double food = 0;
    if (_foodStorage > 0) {
        food += std::min(_foodStorage,
                _AccessTraits().GetSeedFoodStorageConsumption() * elapsedSec);
        _foodStorage = std::max(0., _foodStorage - food);
    }

    food += _rootBase->CollectFood();
    food += _branchBase->CollectFood();

    food = std::max(0., food - _subtreeMaintenanceConsumption);

    double distribution = _AccessTraits().GetSeedFoodDistribution();
    _rootBase->_TickDfs(food * distribution, elapsedSec);
    _branchBase->_TickDfs(food * (1 - distribution), elapsedSec);
}

void NodeSeed::Tick(double elapsedSec) {
    double _;
    Tick(_, elapsedSec);
}

void NodeSeed::Update(World* world) {
    _UpdateDfs(world);
}

double NodeSeed::_GetMaintenanceConsumption() const {
    return SeedMaintenanceConsumption;
}

void NodeSeed::Print(std::ostream& out) const {
    out << "NodeSeed {"
        << " _foodStorage=" << _foodStorage
        << " }";
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

    if (_active && _foodAccumulated > _AccessTraits().GetSproutAccumulatedToGrow()) {
        _foodAccumulated = 0;

        if (ExtMath::RandomDouble(0, 1) > _AccessTraits().GetSproutChanceToSpawnLeaf()) {
            _SpawnBranch();
        } else {
            _SpawnLeaf();
        }

        if (ExtMath::RandomDouble(0, 1) < _AccessTraits().GetSproutChanceToTerminate()) {
            _active = false;
        }
    }
}

void NodeSprout::_SpawnBranch() {
    double deviationMin = _AccessTraits().GetSproutBranchAngleDeviationMin();
    double deviationRange = _AccessTraits().GetSproutBranchAngleDeviationRange();
    int sign = ExtMath::RandomInt(0, 2) * 2 - 1;
    double deltaAngle = ExtMath::RandomDouble(deviationMin, deviationMin + deviationRange) * sign;
    _parent->AddChild(std::make_shared<NodeBranch>(_parent, _position, _angle + deltaAngle, _phenotype));
}

void NodeSprout::_SpawnLeaf() {
    double deviationMin = _AccessTraits().GetSproutLeafAngleDeviationMin();
    double deviationRange = _AccessTraits().GetSproutLeafAngleDeviationRange();
    int sign = ExtMath::RandomInt(0, 2) * 2 - 1;
    double deltaAngle = ExtMath::RandomDouble(deviationMin, deviationMin + deviationRange) * sign;
    _parent->AddChild(std::make_shared<NodeLeaf>(_parent, _position, _angle + deltaAngle, _phenotype));
}

double NodeSprout::_GetMaintenanceConsumption() const {
    return SproutMaintenanceConsumption;
}

void NodeSprout::Print(std::ostream& out) const {
    out << "NodeSprout {"
        << " _active=" << _active
        << " _foodAccumulated=" << _foodAccumulated
        << " }";
}

// ################################################################# //
// ########################## Node BRANCH ########################## //
// ################################################################# //

NodeBranch::NodeBranch(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, phenotype->AccessTraits(parent->GetDepth() + 1).GetBranchInitialLength(), phenotype)
{
    AddChild(std::make_shared<NodeSprout>(this, GetEdge(), angle, _phenotype));
}

double NodeBranch::CollectFood() {
    return 0;
}

void NodeBranch::Tick(double& food, double elapsedSec) {
    double growthConsumption = food * _AccessTraits().GetBranchFoodDistribution();
    food -= growthConsumption;
    _length += growthConsumption * _AccessTraits().GetBranchGrowthSpeed() / _length;
}

double NodeBranch::_GetMaintenanceConsumption() const {
    return _length * BranchMaintenanceConsumptionCoefficient;
}

void NodeBranch::Print(std::ostream& out) const {
    out << "NodeBranch {"
        << " _length=" << _length
        << " }";
}

// ################################################################# //
// ########################### Node LEAF ########################### //
// ################################################################# //

NodeLeaf::NodeLeaf(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle,
            phenotype->AccessTraits(parent->GetDepth() + 1).GetLeafStemLength(),
            phenotype)
    , _brightness(0)
    , _efficiency(_AccessTraits().GetLeafFoodGenerationEfficiency())
{}

double NodeLeaf::CollectFood() {
    return _brightness * _efficiency;
}

void NodeLeaf::Tick(double& food, double elapsedSec) {
    food = 0;
}

void NodeLeaf::SetBrightness(double brightness) {
    _brightness = brightness;
}

double NodeLeaf::_GetMaintenanceConsumption() const {
    return _efficiency * LeafMaintenanceConsumptionCoefficient;
}

void NodeLeaf::Print(std::ostream& out) const {
    out << "NodeLeaf {"
        << " _brightness=" << _brightness
        << " _efficiency=" << _efficiency
        << " }";
}

void NodeLeaf::_Update(World* world) {
    world->AddLeaf(this);
    Node::_Update(world);
}

// ################################################################# //
// ########################### Node ROOT ########################### //
// ################################################################# //

NodeRoot::NodeRoot(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, phenotype->AccessTraits(parent->GetDepth() + 1).GetRootInitialLength(), phenotype)
    , _growthStopped(false)
{
    AddChild(std::make_shared<NodeRootSprout>(this, GetEdge(), angle, _phenotype));
}

double NodeRoot::CollectFood() {
    return 0;
}

void NodeRoot::Tick(double& food, double elapsedSec) {
    if (!_growthStopped) {
        double growthConsumption = food * _AccessTraits().GetRootFoodDistribution();
        food -= growthConsumption;
        _length += growthConsumption * _AccessTraits().GetRootGrowthSpeed() / _length;
    }
}

void NodeRoot::StopGrowth() {
    _growthStopped = true;
}

double NodeRoot::_GetMaintenanceConsumption() const {
    return _length * RootMaintenanceConsumptionCoefficient;
}

void NodeRoot::Print(std::ostream& out) const {
    out << "NodeRoot {"
        << " _length=" << _length
        << " _growthStopped=" << _growthStopped
        << " }";
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

    if (_active && _foodAccumulated > _AccessTraits().GetRootSproutAccumulatedToGrow()) {
        _foodAccumulated = 0;

        if (ExtMath::RandomDouble(0, 1) > _AccessTraits().GetRootSproutChanceToSpawnMiner()) {
            _SpawnMiner();
        } else {
            _SpawnRoot();
        }

        dynamic_cast<NodeRoot*>(_parent)->StopGrowth();

        if (ExtMath::RandomDouble(0, 1) < _AccessTraits().GetRootSproutChanceToTerminate()) {
            _active = false;
        }
    }
}

void NodeRootSprout::_SpawnRoot() {
    double deviationMin = _AccessTraits().GetRootSproutAngleDeviationMin();
    double deviationRange = _AccessTraits().GetRootSproutAngleDeviationRange();
    int sign = ExtMath::RandomInt(0, 2) * 2 - 1;
    double deltaAngle = ExtMath::RandomDouble(deviationMin, deviationMin + deviationRange) * sign;
    _parent->AddChild(std::make_shared<NodeRoot>(_parent, _position, _angle + deltaAngle, _phenotype));
}

void NodeRootSprout::_SpawnMiner() {
    double deviationMin = _AccessTraits().GetRootSproutMinerAngleDeviationMin();
    double deviationRange = _AccessTraits().GetRootSproutMinerAngleDeviationRange();
    int sign = ExtMath::RandomInt(0, 2) * 2 - 1;
    double deltaAngle = ExtMath::RandomDouble(deviationMin, deviationMin + deviationRange) * sign;
    _parent->AddChild(std::make_shared<NodeMiner>(_parent, _position, _angle + deltaAngle, _phenotype));
}

double NodeRootSprout::_GetMaintenanceConsumption() const {
    return RootSproutMaintenanceConsumption;
}

void NodeRootSprout::Print(std::ostream& out) const {
    out << "NodeRootSprout {"
        << " _active=" << _active
        << " _foodAccumulated=" << _foodAccumulated
        << " }";
}

// ################################################################# //
// ########################## Node MINER  ########################## //
// ################################################################# //

NodeMiner::NodeMiner(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, phenotype->AccessTraits(parent->GetDepth() + 1).GetMinerStemLength(), phenotype)
    , _mineralConcentration(0)
    , _efficiency(_AccessTraits().GetMinerFoodGenerationEfficiency())
{}

double NodeMiner::CollectFood() {
    return _mineralConcentration * _efficiency;
}

void NodeMiner::Tick(double& food, double elapsedSec) {
    food = 0;
}

void NodeMiner::SetMineralConcentration(double mineralConcentration) {
    _mineralConcentration = mineralConcentration;
}

double NodeMiner::_GetMaintenanceConsumption() const {
    return _efficiency * MinerMaintenanceConsumptionCoefficient;
}

void NodeMiner::_Update(World* world) {
    world->AddMiner(this);
    Node::_Update(world);
}

void NodeMiner::Print(std::ostream& out) const {
    out << "NodeMiner {"
        << " _mineralConcentration=" << _mineralConcentration
        << " _efficiency=" << _efficiency
        << " }";
}
