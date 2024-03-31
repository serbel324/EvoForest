#pragma once

#include <evo_lib/dna.h>

#include <library/ext_math.h>

#include <forest/tree/gene.h>
#include <forest/global.h>
#include <forest/tree/phenotype.h>

#include <forest/tree/nodes/seed.h>

#include <iostream>
#include <memory>
#include <vector>

class World;

class Tree {

public:
    using SPtr = std::shared_ptr<Tree>;

public:
    Tree(Vec2f position, const Phenotype::SPtr& phenotype);

public:
    void Render(const Renderer::SPtr& renderer) const;
    bool Tick(double elapsedSec, World* world); // returns false if dies on this tick
    void Update(World* world);

private:
    NodeSeed::SPtr _seed;
};
