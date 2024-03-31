#pragma once

#include <evo_lib/dna.h>

#include <library/ext_math.h>

#include <forest/global.h>
#include <forest/tree/phenotype.h>

#include <forest/renderer/renderer.h>

#include <iostream>
#include <memory>
#include <vector>

class World;

class Node {

public:
    using SPtr = std::shared_ptr<Node>;
    using Dna = EvoLib::Dna<TreeGene>;

public:
    Node(Node* parent, Vec2f position, double angle, double length, const Phenotype::SPtr& phenotype);

    virtual ~Node() = default;

    virtual double CollectFood() = 0;
    virtual void Render(const Renderer::SPtr& renderer) const = 0;

    void AddChild(const SPtr& child);

    size_t GetDepth() const;
    double GetSubtreeMaintenanceConsumption() const;
    Vec2f GetPosition() const;
    Vec2f GetEdge() const;

    std::string ToString() const;
    virtual void Print(std::ostream& out) const;

    void PrintSubtree(std::ostream& out, const std::string& prefix = "") const;

protected:
    // TODO think about uniting some of these dfs's
    double _CollectFoodDfs();
    void _TickDfs(double food, double elapsedSec);
    void _RenderDfs(const Renderer::SPtr& renderer) const;
    void _UpdateDfs(World* world);
    void _PrintSubtreeDfs(std::ostream& out);

protected:
    virtual double _GetMaintenanceConsumption() const = 0;
    virtual void _Tick(double& food, double elapsedSec) = 0;
    virtual void _Update(World* world);

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
