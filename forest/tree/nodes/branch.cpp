#include <forest/tree/nodes/branch.h>
#include <forest/tree/nodes/sprout.h>

NodeBranch::NodeBranch(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype)
    : Node(parent, position, angle, phenotype->AccessTraits(parent->GetDepth() + 1).GetBranchInitialLength(), phenotype)
{
    AddChild(std::make_shared<NodeSprout>(this, GetEdge(), angle, _phenotype));
}

double NodeBranch::CollectFood() {
    return 0;
}

void NodeBranch::_Tick(double& food, double elapsedSec) {
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
