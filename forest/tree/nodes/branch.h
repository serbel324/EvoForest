#pragma once

#include <forest/tree/nodes/base.h>

class NodeBranch : public Node {
public:
    using SPtr = std::shared_ptr<NodeBranch>;
    friend class NodeSeed;

public:
    NodeBranch(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

public:
    double CollectFood() override;
    void Print(std::ostream& out) const override;
    void Render(const Renderer::SPtr& renderer) const override;

protected:
    double _GetMaintenanceConsumption() const override;
    void _Tick(double& food, double elapsedSec, World* /*world*/) override;
};
