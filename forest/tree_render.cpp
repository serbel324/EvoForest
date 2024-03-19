#include <forest/tree.h>

#include <rengine/core/graphics.h>

#include <iostream>

void NodeSeed::Render(REngine::Graphics* gr) const {
    gr->SetFillColor(REngine::Color(200, 200, 140));
    gr->DrawCircle(_position, 5);

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
    gr->DrawLine(_position, _GetEdge());
}

void NodeRoot::Render(REngine::Graphics* gr) const {
    gr->SetFillColor(REngine::Color::BLACK);
    gr->DrawLine(_position, _GetEdge());
    gr->DrawCircle(_position, 5);
}
