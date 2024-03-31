#include <forest/tree/nodes/branch.h>
#include <forest/tree/nodes/leaf.h>
#include <forest/tree/nodes/sprout.h>

NodeSprout::NodeSprout(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, 0, phenotype)
    , _active(true)
    , _foodAccumulated(0)
{}

double NodeSprout::CollectFood() {
    return 0;
}

void NodeSprout::_Tick(double& food, double elapsedSec) {
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
