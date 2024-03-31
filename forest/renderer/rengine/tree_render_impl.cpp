#include <forest/tree/nodes/branch.h>
#include <forest/tree/nodes/leaf.h>
#include <forest/tree/nodes/miner.h>
#include <forest/tree/nodes/root.h>
#include <forest/tree/nodes/root_sprout.h>
#include <forest/tree/nodes/seed.h>
#include <forest/tree/nodes/sprout.h>

#include <forest/renderer/rengine/rengine_renderer.h>

void NodeSeed::Render(const Renderer::SPtr& renderer) const {
    REngine::Graphics* gr = renderer->As<REngineRenderer>()->gr;
    gr->SetFillColor(REngine::Color::MAGENTA);
    gr->DrawCircle(_position, 2);

    _branchBase->_RenderDfs(renderer);
    _rootBase->_RenderDfs(renderer);
}

void NodeSprout::Render(const Renderer::SPtr& renderer) const {
    REngine::Graphics* gr = renderer->As<REngineRenderer>()->gr;
    if (_active) {
        gr->SetFillColor(REngine::Color::RED);
        gr->DrawCircle(_position, 2);
    }
}

void NodeBranch::Render(const Renderer::SPtr& renderer) const {
    REngine::Graphics* gr = renderer->As<REngineRenderer>()->gr;
    gr->SetFillColor(REngine::Color(130, 80, 30));
    gr->DrawLine(_position, GetEdge());
}

void NodeLeaf::Render(const Renderer::SPtr& renderer) const {
    REngine::Graphics* gr = renderer->As<REngineRenderer>()->gr;
    gr->SetFillColor(REngine::Color::GREEN * (_brightness * 0.01));
    gr->DrawCircle(GetEdge(), 3);
    gr->DrawLine(_position, GetEdge());
}

void NodeRoot::Render(const Renderer::SPtr& renderer) const {
    REngine::Graphics* gr = renderer->As<REngineRenderer>()->gr;
    gr->SetFillColor(REngine::Color::BLACK);
    gr->DrawLine(_position, GetEdge());
}

void NodeRootSprout::Render(const Renderer::SPtr& renderer) const {
    REngine::Graphics* gr = renderer->As<REngineRenderer>()->gr;
    if (_active) {
        gr->SetFillColor(REngine::Color::RED);
        gr->DrawCircle(_position, 2);
    }
}

void NodeMiner::Render(const Renderer::SPtr& renderer) const {
    REngine::Graphics* gr = renderer->As<REngineRenderer>()->gr;
    gr->SetFillColor(REngine::Color::YELLOW * (_mineralConcentration * 0.01));
    gr->DrawCircle(GetEdge(), 3);
    gr->DrawLine(_position, GetEdge());
}
