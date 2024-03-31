#pragma once

#include <forest/tree/nodes/base.h>

class NodeSprout : public Node {
public:
    NodeSprout(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

public:
    double CollectFood() override;
    void Print(std::ostream& out) const override;
    void Render(const Renderer::SPtr& renderer) const override;

protected:
    double _GetMaintenanceConsumption() const override;
    void _Tick(double& food, double elapsedSec, World* /*world*/) override;

private:
    double _foodAccumulated;
    double _foodToSprout;
    bool _active;

private:
    void _SpawnBranch();
    void _SpawnLeaf();
};
