#include <forest/tree/nodes/root.h>
#include <forest/tree/nodes/root_sprout.h>

NodeRoot::NodeRoot(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, phenotype->AccessTraits(parent->GetDepth() + 1).GetRootInitialLength(), phenotype)
    , _growthStopped(false)
{
    AddChild(std::make_shared<NodeRootSprout>(this, GetEdge(), angle, _phenotype));
}

double NodeRoot::CollectFood() {
    return 0;
}

void NodeRoot::_Tick(double& food, double elapsedSec) {
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
