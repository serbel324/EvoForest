#include <forest/tree/tree.h>

Tree::Tree(Vec2f position, const Phenotype::SPtr& phenotype)
    : _seed(new NodeSeed(position, phenotype))
{}

void Tree::Tick(double elapsedSec) {
    _seed->Tick(elapsedSec);
}

void Tree::Update(World* world) {
    _seed->_UpdateDfs(world);
}

void Tree::Render(const Renderer::SPtr& renderer) const {
    _seed->_RenderDfs(renderer);
}
