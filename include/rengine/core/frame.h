#pragma once
#include <memory>
#include <string>

#include <rengine/core/graphics.h>
#include <rengine/core/input.h>
#include <library/vec2.h>

#include <SFML/Graphics.hpp>

namespace REngine {
std::shared_ptr<sf::RenderWindow> MakeGenericWindow(Vec2i dimensions, std::string name = "App");

class Frame {
public:
    using UPtr = std::unique_ptr<Frame>;
    using SPtr = std::shared_ptr<Frame>;

    using Id = std::string;

public:
    struct Settings {
        Id id;
        Vec2<size_t> screenSize;
    };

public:
    Frame(Settings settings, Frame* parent = nullptr, std::shared_ptr<sf::RenderWindow> window = {});

    virtual void Initialize() = 0;
    virtual bool Update(float elapsedMs);
    virtual void Render();
    virtual void PollEvents();

    virtual ~Frame() = default;

protected:
    Graphics* Gr();
    InputController* Ic();

    bool IsRunning() const;

protected:
    const Settings _settings;
    Frame* _parent;
    std::shared_ptr<sf::RenderWindow> _window;

private:
    std::shared_ptr<Graphics> _graphics;
    std::shared_ptr<InputController> _inputController;

    bool _isRunning;
};

} // namespace REngine
