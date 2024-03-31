#include <forest/world.h>
#include <forest/rengine_impl/rengine_renderer.h>

void World::Render(const Renderer::SPtr& renderer) const {
    REngine::Graphics* gr = renderer->As<REngineRenderer>()->gr;
    gr->SetFillColor(REngine::Color(190, 240, 255));
    gr->Fill();
    gr->SetFillColor(REngine::Color(140, 120, 80));
    gr->DrawRect(Vec2f{-10000, 0}, Vec2f{20000, 1000});

    for (const Tree::SPtr& tree : _trees) {
        tree->Render(renderer);
    }
}
