#include <forest/world.h>

#include <algorithm>

#include <iostream>

void LightRay::Sort() {
    std::sort(leaves.begin(), leaves.end(),
        [](const NodeLeaf* left, const NodeLeaf* right) {
            return left->GetEdge().y < right->GetEdge().y;
        }
    );
}

World::World(Rectangle<double> worldBorders)
    : _worldBorders(worldBorders)
{}

void World::Init() {
    _SpawnTrees();
}

void World::Tick(double elapsedSec) {
    _timeService.Tick(elapsedSec);

    _rays.clear();
    _soilChunks.clear();

    for (const Tree::SPtr& tree : _trees) {
        tree->Update(this);
    }

    _UpdateLighting();
    _UpdateSoil();

    for (auto it = _trees.begin(); it != _trees.end(); ++it) {
        if (!(*it)->Tick(elapsedSec, this)) {
            it->reset();

            _timeService.PlanEvent(0, [this]() {
                Vec2f pos{ExtMath::RandomDouble(_worldBorders.left, _worldBorders.right), 0.};
                _SpawnRandomTree(pos);
            });
        }
    }

    _trees.erase(
        std::remove_if(
            _trees.begin(),
            _trees.end(),
            [](const Tree::SPtr& p) { return !p; }
        ),
        _trees.end()
    );
}

void World::_SpawnTrees() {
    _trees.clear();

    for (size_t i = 0; i < _initialTreesNumber; ++i) {
        Vec2f pos{ExtMath::RandomDouble(_worldBorders.left, _worldBorders.right), 0.};
        _SpawnRandomTree(pos);
    }
}

void World::AddLeaf(NodeLeaf* leaf) {
    int rayIdx = std::round(leaf->GetEdge().x / LightRayResolution);
    _rays[rayIdx].leaves.emplace_back(leaf);
}

void World::AddMiner(NodeMiner* miner) {
    Vec2f pos = miner->GetEdge();
    int xIdx = std::round(pos.x / SoilChunkSize);
    int yIdx = std::round(pos.y / SoilChunkSize);
    _soilChunks[Vec2i{xIdx, yIdx}].emplace_back(miner);
}

void World::_UpdateLighting() {
    for (auto& [idx, ray] : _rays) {
        ray.Sort();
        double brightness = LightIntensity;
        for (NodeLeaf* leaf : ray.leaves) {
            if (leaf->GetEdge().y > 0) {
                leaf->SetBrightness(0); 
            } else {
                leaf->SetBrightness(brightness);
                brightness = std::max(brightness - LeafShadow, 0.);
            }
        }
    }
}

void World::_UpdateSoil() {
    for (auto& [idx, chunk] : _soilChunks) {
        double mineralsPerMiner = SoilMineralConcentration / chunk.size();
        for (NodeMiner* miner : chunk) {
            if (miner->GetEdge().y < 0) {
                miner->SetMineralConcentration(0); 
            } else {
                miner->SetMineralConcentration(mineralsPerMiner);
            }
        }
    }
}

void World::_SpawnRandomTree(Vec2f pos) {
    Phenotype::Dna::SPtr dna;
    Phenotype::SPtr phenotype(new Phenotype(dna));
    _trees.emplace_back(new Tree(pos, phenotype));
}
