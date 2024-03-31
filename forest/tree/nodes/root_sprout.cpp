#include <forest/tree/nodes/miner.h>
#include <forest/tree/nodes/root.h>
#include <forest/tree/nodes/root_sprout.h>

NodeRootSprout::NodeRootSprout(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, 0, phenotype)
    , _active(true)
    , _foodAccumulated(0)
{}

double NodeRootSprout::CollectFood() {
    return 0;
}

void NodeRootSprout::_Tick(double& food, double elapsedSec, World* /*world*/) {
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
