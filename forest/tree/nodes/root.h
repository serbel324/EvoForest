#pragma once

#include <forest/tree/nodes/base.h>

class NodeRoot : public Node {
public:
    using SPtr = std::shared_ptr<NodeRoot>;
    friend class NodeSeed;

public:
    NodeRoot(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

    void StopGrowth();

public:
    double CollectFood() override;
    void Print(std::ostream& out) const override;
    void Render(const Renderer::SPtr& renderer) const override;

protected:
    double _GetMaintenanceConsumption() const override;
    void _Tick(double& food, double elapsedSec) override;

public:
    bool _growthStopped;
};
