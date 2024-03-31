#include <forest/tree/tree.h>

Tree::Tree(Vec2f position, const Phenotype::SPtr& phenotype)
    : _seed(new NodeSeed(position, phenotype))
{}

bool Tree::Tick(double elapsedSec, World* world) {
    return _seed->Tick(elapsedSec, world);
}

void Tree::Update(World* world) {
    _seed->_UpdateSubtree(world);
}

void Tree::Render(const Renderer::SPtr& renderer) const {
    _seed->_RenderSubtree(renderer);
}
