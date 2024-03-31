#pragma once

#include <forest/tree/nodes/base.h>
#include <forest/tree/nodes/branch.h>
#include <forest/tree/nodes/root.h>

class NodeSeed : public Node {
public:
    using SPtr = std::shared_ptr<NodeSeed>;

    friend class Tree;

public:
    NodeSeed(Vec2f position, const Phenotype::SPtr& phenotype);

public:
    double CollectFood() override;
    void Print(std::ostream& out) const override;
    void Render(const Renderer::SPtr& renderer) const override;
    bool Tick(double elapsedSec);

protected:
    double _GetMaintenanceConsumption() const override;
    void _Tick(double& food, double elapsedSec) override;

private:
    NodeBranch::SPtr _branchBase;
    NodeRoot::SPtr _rootBase;

    double _foodStorage;

    // DEBUG
    double _foodProducing;
    double _foodSpending;

    bool _alive;
};
