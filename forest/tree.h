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
*         SUCKER,
* };
*/

public:
    using SPtr = std::shared_ptr<Node>;
    using Dna = EvoLib::Dna<TreeGene>;

public:
    Node(Node* parent, Vec2f position, float angle, float length, const Phenotype::SPtr& phenotype);

    virtual ~Node() = default;

    virtual float CollectFood() = 0;
    virtual void Tick(float& food, float elapsedSec) = 0;
    virtual void Render(REngine::Graphics* gr) const = 0;

    void AddChild(const SPtr& child);

protected:
    float _CollectFoodDfs();
    void _TickDfs(float food, float elapsedSec);
    void _RenderDfs(REngine::Graphics* gr) const;


protected:
    Vec2f _GetEdge() const;

protected:
    Vec2f _position; // base of the node
    float _angle;
    float _length;

    Node* _parent;
    std::vector<SPtr> _children;
    Phenotype::SPtr _phenotype;
};

class NodeSprout : public Node {
public:
    NodeSprout(Node* parent, Vec2f position, float angle, const Phenotype::SPtr& phenotype);

    float CollectFood() override;
    void Tick(float& food, float elapsedSec) override;
    virtual void Render(REngine::Graphics* gr) const override;

private:
    float _foodAccumulated;
    float _foodToSprout;
    bool _active;
};

class NodeBranch : public Node {
public:
    using SPtr = std::shared_ptr<NodeBranch>;
    friend class NodeSeed;

public:
    NodeBranch(Node* parent, Vec2f position, float angle, const Phenotype::SPtr& phenotype);

    float CollectFood() override;
    void Tick(float& food, float elapsedSec) override;
    virtual void Render(REngine::Graphics* gr) const override;
};


class NodeRoot : public Node {
public:
    using SPtr = std::shared_ptr<NodeRoot>;
    friend class NodeSeed;

public:
    NodeRoot(Node* parent, Vec2f position, float angle, const Phenotype::SPtr& phenotype);

    float CollectFood() override;
    void Tick(float& food, float elapsedSec) override;
    virtual void Render(REngine::Graphics* gr) const override;
};

class NodeSeed : public Node {
public:
    NodeSeed(Vec2f position, const Phenotype::SPtr& phenotype);

    float CollectFood() override;
    void Tick(float& food, float elapsedSec) override;
    virtual void Render(REngine::Graphics* gr) const override;

private:
    NodeBranch::SPtr _branchBase;
    NodeRoot::SPtr _rootBase;

    float _foodStorage;
    float _foodSpending;
};
