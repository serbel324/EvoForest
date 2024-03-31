#pragma once

#include <library/util.h>

#include <forest/global.h>
#include <forest/tree/tree.h>
#include <forest/tree/nodes/leaf.h>
#include <forest/tree/nodes/miner.h>
#include <forest/tree/nodes/seed.h>

#include <forest/renderer/renderer.h>

#include <vector>
#include <unordered_map>

struct LightRay {
    std::vector<NodeLeaf*> leaves;
    void Sort();
};

using SoilChunk = std::vector<NodeMiner*>;

class World {
public:
    World(Rectangle<double> worldBorders);

public:
    void Init();
    void Tick(double elapsedSec);
    void Render(const Renderer::SPtr& renderer) const;

    void AddLeaf(NodeLeaf* leaf);
    void AddMiner(NodeMiner* miner);

private:
    Rectangle<double> _worldBorders;
    std::vector<std::shared_ptr<Tree>> _trees;

    float _timerSec;
    uint32_t _treesNumber = 10;

    std::unordered_map<int, LightRay> _rays;
    std::unordered_map<Vec2i, std::vector<NodeMiner*>> _soilChunks;

private:
    void _SpawnTrees();

    void _UpdateLighting();
    void _UpdateSoil();
};
