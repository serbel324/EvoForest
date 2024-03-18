#include <rengine/core/frame.h>
#include <rengine/core/color.h>
#include <library/vec2.h>

#include <fstream>

#include <forest/tree.h>

using namespace REngine;

const Vec2i _worldSize{300, 300};
const Vec2i _screenSize{800, 800};

class MainFrame : public Frame {
public:
    MainFrame()
        : Frame(
            Frame::Settings{
                .id = "MainFrame",
                .screenSize = _screenSize,
            },
            nullptr,
            MakeGenericWindow(_screenSize, "EvoLib"))
    {}

    void Initialize() override {
        std::cout << "Init" << std::endl;
        Phenotype::Dna::SPtr dna;
        Phenotype::SPtr phenotype(new Phenotype(dna));
        _camera.reset(new REngine::Camera(Vec2f{-400, -400}));
        Gr()->SetCamera(_camera);
        _tree.reset(new NodeSeed(Vec2f{0, 0}, phenotype));
    }

    bool Update(float elapsedMs) override {
        float _;
        _tree->Tick(_, elapsedMs / 1000);
        return Frame::Update(elapsedMs);
    }

    void Render() override {
        Gr()->SetFillColor(REngine::Color::WHITE);
        Gr()->Fill();

        _tree->Render(Gr());

        Frame::Render();
    }

private:
    std::shared_ptr<NodeSeed> _tree;
    REngine::Camera::SPtr _camera;
};
