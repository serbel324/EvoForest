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
    gr->SetFillColor(REngine::Color::GREEN);
    gr->DrawCircle(_position, 5);
}

void NodeBranch::Render(REngine::Graphics* gr) const {
    gr->SetFillColor(REngine::Color(130, 80, 30));
    auto [pos, a] = GetEdge();
    std::cout << "Position# " << _position << " Edge# " << pos << std::endl;
    gr->DrawLine(_position, pos);
    gr->DrawCircle(_position, 5);
}

void NodeRoot::Render(REngine::Graphics* gr) const {
    gr->SetFillColor(REngine::Color::BLACK);
    auto [pos, a] = GetEdge();
    gr->DrawLine(_position, pos);
    gr->DrawCircle(_position, 5);
}
