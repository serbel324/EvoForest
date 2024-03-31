#include <forest/tree/nodes/seed.h>

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

void NodeSeed::_Tick(double&/* food*/, double elapsedSec) {
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
    _Tick(_, elapsedSec);
}

double NodeSeed::_GetMaintenanceConsumption() const {
    return SeedMaintenanceConsumption;
}

void NodeSeed::Print(std::ostream& out) const {
    out << "NodeSeed {"
        << " _foodStorage=" << _foodStorage
        << " }";
}
