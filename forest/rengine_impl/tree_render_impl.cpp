#include <forest/tree/nodes/branch.h>
#include <forest/tree/nodes/leaf.h>
#include <forest/tree/nodes/miner.h>
#include <forest/tree/nodes/root.h>
#include <forest/tree/nodes/root_sprout.h>
#include <forest/tree/nodes/seed.h>
#include <forest/tree/nodes/sprout.h>

#include <forest/rengine_impl/rengine_renderer.h>

#include <sstream>

void NodeSeed::Render(const Renderer::SPtr& renderer) const {
    static std::optional<sf::Font> font;

    // DEBUG TODO: enable either config or some keyboard input
    if (!font) {
        font.emplace(sf::Font{});
        font->loadFromFile("font.ttf");
    }

    REngine::Graphics* gr = renderer->As<REngineRenderer>()->gr;
    gr->SetFillColor(REngine::Color::MAGENTA);
    gr->DrawCircle(_position, 2);

    _branchBase->_RenderSubtree(renderer);
    _rootBase->_RenderSubtree(renderer);

    if (font) {
        gr->SetFillColor(REngine::Color::BLACK);
        std::stringstream s;
        s << "[" << _foodStorage << "/" << _foodProducing << "-" << _foodSpending << "]"; 
        gr->FillText(s.str(), _position.x, _position.y + 50, 10, *font);
    }
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
    gr->SetFillColor(REngine::Color::GREEN * (_brightness / LightIntensity));
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
    gr->SetFillColor(REngine::Color::YELLOW * (_mineralConcentration / SoilMineralConcentration));
    gr->DrawCircle(GetEdge(), 3);
    gr->DrawLine(_position, GetEdge());
}
