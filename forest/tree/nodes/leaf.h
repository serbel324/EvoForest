#pragma once

#include <forest/tree/nodes/base.h>

class Node;

class NodeLeaf : public Node {
public:
    using SPtr = std::shared_ptr<NodeLeaf>;

public:
    NodeLeaf(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

    void SetBrightness(double brightness);

public:
    double CollectFood() override;
    void Print(std::ostream& out) const override;
    void Render(const Renderer::SPtr& renderer) const override;

protected:
    double _GetMaintenanceConsumption() const override;
    void _Tick(double& food, double elapsedSec) override;
    void _Update(World* world) override;

private:
    double _brightness;
    double _efficiency;
};
