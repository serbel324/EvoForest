#pragma once

#include <evo_lib/dna.h>

#include <rengine/core/graphics.h>

#include <library/ext_math.h>

#include <forest/gene.h>
#include <forest/global.h>
#include <forest/phenotype.h>

#include <iostream>
#include <memory>
#include <vector>

class Node {


/*
* public:
*     enum class Type {
*         SEED = 0,
*         BRANCH,
*         SPROUT,
*         LEAF,
*         FRUIT,
*         ROOT,
*         ROOT_SPROUT,
*         MINER,
* };
*/

public:
    using SPtr = std::shared_ptr<Node>;
    using Dna = EvoLib::Dna<TreeGene>;

public:
    Node(Node* parent, Vec2f position, double angle, double length, const Phenotype::SPtr& phenotype);

    virtual ~Node() = default;

    virtual double CollectFood() = 0;
    virtual void Tick(double& food, double elapsedSec) = 0;
    virtual void Render(REngine::Graphics* gr) const = 0;

    void AddChild(const SPtr& child);

    size_t GetDepth() const;
    double GetSubtreeMaintenanceConsumption() const;

    std::string ToString() const;
    virtual void Print(std::ostream& out) const;

    void PrintSubtree(std::ostream& out, const std::string& prefix = "") const;

protected:
    double _CollectFoodDfs();
    void _TickDfs(double food, double elapsedSec);
    void _RenderDfs(REngine::Graphics* gr) const;
    void _UpdateDfs();
    void _PrintSubtreeDfs(std::ostream& out);

protected:
    void _Update();
    Vec2f _GetEdge() const;
    virtual double _GetMaintenanceConsumption() const = 0;

    const Phenotype::TraitAccessor& _AccessTraits() const;

protected:
    size_t _depth;
    Vec2f _position; // base of the node

    double _angle;
    double _length;
    double _maintenanceConsumption;
    double _subtreeMaintenanceConsumption;

protected:
    Node* _parent;
    std::vector<SPtr> _children;
    Phenotype::SPtr _phenotype;
};


class NodeSprout : public Node {
public:
    NodeSprout(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

public:
    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    void Render(REngine::Graphics* gr) const override;
    void Print(std::ostream& out) const override;

protected:
    double _GetMaintenanceConsumption() const override;

private:
    double _foodAccumulated;
    double _foodToSprout;
    bool _active;

private:
    void _SpawnBranch();
    void _SpawnLeaf();
};


class NodeBranch : public Node {
public:
    using SPtr = std::shared_ptr<NodeBranch>;
    friend class NodeSeed;

public:
    NodeBranch(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

public:
    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    void Render(REngine::Graphics* gr) const override;
    void Print(std::ostream& out) const override;

protected:
    double _GetMaintenanceConsumption() const override;
};


class NodeLeaf : public Node {
public:
    NodeLeaf(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

    void SetBrightness(double brightness);

public:
    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    void Render(REngine::Graphics* gr) const override;
    void Print(std::ostream& out) const override;

protected:
    double _GetMaintenanceConsumption() const override;

private:
    double _brightness;
    double _efficiency;
};


class NodeRoot : public Node {
public:
    using SPtr = std::shared_ptr<NodeRoot>;
    friend class NodeSeed;

public:
    NodeRoot(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

    void StopGrowth();

public:
    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    void Render(REngine::Graphics* gr) const override;
    void Print(std::ostream& out) const override;

protected:
    double _GetMaintenanceConsumption() const override;

public:
    bool _growthStopped;
};


class NodeRootSprout : public Node {
public:
    NodeRootSprout(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    void Render(REngine::Graphics* gr) const override;
    void Print(std::ostream& out) const override;

protected:
    double _GetMaintenanceConsumption() const override;

private:
    double _foodAccumulated;
    double _foodToSprout;
    bool _active;

private:
    void _SpawnRoot();
    void _SpawnMiner();
};


class NodeMiner : public Node {
public:
    NodeMiner(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

    void SetMineralConcentration(double mineralConcentraion);

public:
    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    void Render(REngine::Graphics* gr) const override;
    void Print(std::ostream& out) const override;

protected:
    double _GetMaintenanceConsumption() const override;

private:
    double _mineralConcentration;
    double _efficiency;
};


class NodeSeed : public Node {
public:
    NodeSeed(Vec2f position, const Phenotype::SPtr& phenotype);

    void Tick(double elapsedSec);

public:
    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    void Render(REngine::Graphics* gr) const override;
    void Print(std::ostream& out) const override;
    void Update();

protected:
    double _GetMaintenanceConsumption() const override;

private:
    NodeBranch::SPtr _branchBase;
    NodeRoot::SPtr _rootBase;

    double _foodStorage;
    double _foodSpending;
};
