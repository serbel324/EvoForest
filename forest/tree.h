#pragma once

#include <evo_lib/dna.h>

#include <rengine/core/graphics.h>

#include <library/ext_math.h>

#include <forest/gene.h>
#include <forest/phenotype.h>

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

protected:
    double _CollectFoodDfs();
    void _TickDfs(double food, double elapsedSec);
    void _RenderDfs(REngine::Graphics* gr) const;


protected:
    Vec2f _GetEdge() const;

protected:
    size_t _depth;
    Vec2f _position; // base of the node
    double _angle;
    double _length;

    Node* _parent;
    std::vector<SPtr> _children;
    Phenotype::SPtr _phenotype;
};


class NodeSprout : public Node {
public:
    NodeSprout(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    virtual void Render(REngine::Graphics* gr) const override;

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

    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    virtual void Render(REngine::Graphics* gr) const override;
};


class NodeLeaf : public Node {
public:
    NodeLeaf(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    virtual void Render(REngine::Graphics* gr) const override;

    void SetBrightness(double brightness);

private:
    double _brightness;
};


class NodeRoot : public Node {
public:
    using SPtr = std::shared_ptr<NodeRoot>;
    friend class NodeSeed;

public:
    NodeRoot(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    virtual void Render(REngine::Graphics* gr) const override;

    void StopGrowth();

public:
    bool _growthStopped;
};


class NodeRootSprout : public Node {
public:
    NodeRootSprout(Node* parent, Vec2f position, double angle, const Phenotype::SPtr& phenotype);

    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    virtual void Render(REngine::Graphics* gr) const override;

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

    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    virtual void Render(REngine::Graphics* gr) const override;

    void SetMineralConcentration(double mineralConcentraion);

private:
    double _mineralConcentration;
};


class NodeSeed : public Node {
public:
    NodeSeed(Vec2f position, const Phenotype::SPtr& phenotype);

    double CollectFood() override;
    void Tick(double& food, double elapsedSec) override;
    virtual void Render(REngine::Graphics* gr) const override;

private:
    NodeBranch::SPtr _branchBase;
    NodeRoot::SPtr _rootBase;

    double _foodStorage;
    double _foodSpending;
};
