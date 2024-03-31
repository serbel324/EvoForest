#include <forest/tree/nodes/seed.h>

NodeSeed::NodeSeed(Vec2f position, const Phenotype::SPtr& phenotype)
    : Node(nullptr, position, 0, 0, phenotype)
    , _branchBase(new NodeBranch(this, position, ExtMath::PI * -0.5, phenotype))
    , _rootBase(new NodeRoot(this, position, ExtMath::PI * 0.5, phenotype))
    , _alive(true)
{
    AddChild(_branchBase);
    AddChild(_rootBase);
    _foodStorage = _AccessTraits().GetFruitFoodStorage();
}

double NodeSeed::CollectFood() {
    return 0;
}

void NodeSeed::_Tick(double&/* food*/, double elapsedSec, World* world) {
    double food = 0;
    if (_foodStorage > 0) {
        food += std::min(_foodStorage,
                _AccessTraits().GetSeedFoodStorageConsumption() * elapsedSec);
        _foodStorage = std::max(0., _foodStorage - food);
    }

    food += std::min(_rootBase->_CollectFoodSubtree(), _branchBase->_CollectFoodSubtree());

    _foodProducing = food;
    _foodSpending = _subtreeMaintenanceConsumption;

    food = std::max(0., food - _subtreeMaintenanceConsumption);

    if (food <= 0) {
        _alive = false;
        return;
    }

    double distribution = _AccessTraits().GetSeedFoodDistribution();
    _rootBase->_TickSubtree(food * distribution, elapsedSec, world);
    _branchBase->_TickSubtree(food * (1 - distribution), elapsedSec, world);
}

bool NodeSeed::Tick(double elapsedSec, World* world) {
    double _;
    _Tick(_, elapsedSec, world);
    return _alive;
}

double NodeSeed::_GetMaintenanceConsumption() const {
    return SeedMaintenanceConsumption;
}

void NodeSeed::Print(std::ostream& out) const {
    out << "NodeSeed {"
        << " _foodStorage=" << _foodStorage
        << " }";
}
