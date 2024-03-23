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
        _camera.reset(new REngine::Camera(Vec2f{-400, -400}));
        Gr()->SetCamera(_camera);
        Regenerate();
    }

    bool Update(float elapsedMs) override {
        double elapsedSec = elapsedMs / 1000;

        _tree->Update();
        _tree->Tick(elapsedSec);

        timer += elapsedSec;
        if (timer > 3) {
            timer = 0;
            Regenerate();
        }

        return Frame::Update(elapsedMs);
    }

    void Render() override {
        Gr()->SetFillColor(REngine::Color(190, 240, 255));
        Gr()->Fill();
        Gr()->SetFillColor(REngine::Color(140, 120, 80));
        Gr()->DrawRect(Vec2f{-10000, 0}, Vec2f{20000, 1000});

        _tree->Render(Gr());

        Frame::Render();
    }

    void Regenerate() {
        if (_tree) {
            std::cout << std::endl;
            _tree->PrintSubtree(std::cout);
        }
        Phenotype::Dna::SPtr dna;
        Phenotype::SPtr phenotype(new Phenotype(dna));
        _tree.reset(new NodeSeed(Vec2f{0, 0}, phenotype));
    }

private:
    float timer = 0;
    std::shared_ptr<NodeSeed> _tree;
    REngine::Camera::SPtr _camera;
};
