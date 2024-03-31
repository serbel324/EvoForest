#include <forest/tree/nodes/miner.h>
#include <forest/world.h>

NodeMiner::NodeMiner(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, phenotype->AccessTraits(parent->GetDepth() + 1).GetMinerStemLength(), phenotype)
    , _mineralConcentration(0)
    , _efficiency(_AccessTraits().GetMinerFoodGenerationEfficiency())
{}

double NodeMiner::CollectFood() {
    return _mineralConcentration * _efficiency;
}

void NodeMiner::_Tick(double& food, double elapsedSec) {
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
