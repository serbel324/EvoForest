#include <rengine/core/frame.h>
#include <rengine/core/color.h>
#include <library/vec2.h>

#include <fstream>

#include <forest/tree.h>
#include <forest/world.h>

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
        , _world(Rectangle<double>{-400, 400, -400, 400})
    {}

    void Initialize() override {
        std::cout << "Init" << std::endl;
        _camera.reset(new REngine::Camera(Vec2f{-400, -400}));
        Gr()->SetCamera(_camera);
        _world.Init();
    }

    bool Update(float elapsedMs) override {
        double elapsedSec = elapsedMs / 1000;

        _world.Tick(elapsedSec);

        return Frame::Update(elapsedMs);
    }

    void Render() override {
        _world.Render(Gr());
        Frame::Render();
    }

private:
    REngine::Camera::SPtr _camera;
    World _world;
};
