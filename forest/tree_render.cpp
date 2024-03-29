#include <forest/tree.h>

#include <rengine/core/graphics.h>

#include <iostream>

void NodeSeed::Render(REngine::Graphics* gr) const {
    gr->SetFillColor(REngine::Color::MAGENTA);
    gr->DrawCircle(_position, 2);

    _branchBase->_RenderDfs(gr);
    _rootBase->_RenderDfs(gr);
}

void NodeSprout::Render(REngine::Graphics* gr) const {
    if (_active) {
        gr->SetFillColor(REngine::Color::RED);
        gr->DrawCircle(_position, 2);
    }
}

void NodeBranch::Render(REngine::Graphics* gr) const {
    gr->SetFillColor(REngine::Color(130, 80, 30));
    gr->DrawLine(_position, GetEdge());
}

void NodeLeaf::Render(REngine::Graphics* gr) const {
    gr->SetFillColor(REngine::Color::GREEN * _brightness);
    gr->DrawCircle(GetEdge(), 3);
    gr->DrawLine(_position, GetEdge());
}

void NodeRoot::Render(REngine::Graphics* gr) const {
    gr->SetFillColor(REngine::Color::BLACK);
    gr->DrawLine(_position, GetEdge());
}

void NodeRootSprout::Render(REngine::Graphics* gr) const {
    if (_active) {
        gr->SetFillColor(REngine::Color::RED);
        gr->DrawCircle(_position, 2);
    }
}

void NodeMiner::Render(REngine::Graphics* gr) const {
    gr->SetFillColor(REngine::Color::YELLOW * _mineralConcentration);
    gr->DrawCircle(GetEdge(), 3);
    gr->DrawLine(_position, GetEdge());
}
