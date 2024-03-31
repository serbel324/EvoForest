#pragma once

#include <forest/tree/nodes/base.h>

class NodeMiner : public Node {
public:
    using SPtr = std::shared_ptr<NodeMiner>;

public:
    NodeMiner(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

    void SetMineralConcentration(double mineralConcentraion);

public:
    double CollectFood() override;
    void Print(std::ostream& out) const override;
    void Render(const Renderer::SPtr& renderer) const override;

protected:
    double _GetMaintenanceConsumption() const override;
    void _Tick(double& food, double elapsedSec) override;
    void _Update(World* world) override;

private:
    double _mineralConcentration;
    double _efficiency;
};
