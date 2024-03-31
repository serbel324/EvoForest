#include <forest/tree/nodes/leaf.h>
#include <forest/world.h>

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

void NodeLeaf::_Tick(double& food, double elapsedSec) {
    food = 0;
}

void NodeLeaf::SetBrightness(double brightness) {
    _brightness = brightness;
}

double NodeLeaf::_GetMaintenanceConsumption() const {
    return _efficiency * LeafMaintenanceConsumptionCoefficient;
}

void NodeLeaf::_Update(World* world) {
    world->AddLeaf(this);
    Node::_Update(world);
}

void NodeLeaf::Print(std::ostream& out) const {
    out << "NodeLeaf {"
        << " _brightness=" << _brightness
        << " _efficiency=" << _efficiency
        << " }";
}
