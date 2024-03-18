#include <rengine/core/frame.h>

namespace REngine {

std::shared_ptr<sf::RenderWindow> MakeGenericWindow(Vec2i dimensions, std::string name) {
    return std::make_shared<sf::RenderWindow>(sf::VideoMode(dimensions.x, dimensions.y), name, sf::Style::Close);
}

Frame::Frame(Frame::Settings settings, Frame* parent, std::shared_ptr<sf::RenderWindow> window)
    : _settings(std::move(settings))
    , _parent(parent)
    , _window(window)
    , _isRunning(true)
{
    if (parent) {
        _graphics = parent->_graphics;
        _inputController = parent->_inputController;
        if (!window) {
            _window = parent->_window;
        }
    } else {
        _graphics = std::make_shared<Graphics>(_window, _settings.screenSize);
        _inputController = std::make_shared<InputController>();
    }
}

Graphics* Frame::Gr() {
    return _graphics.get();
}

InputController* Frame::Ic() {
    return _inputController.get();
}

bool Frame::IsRunning() const {
    return _isRunning;
}

void Frame::Render() {
    Gr()->Present();
}

bool Frame::Update(float) {
    if (_inputController) {
        PollEvents();
    }

    return _isRunning;
}

void Frame::PollEvents() {
    sf::Event e;

    while (_window->pollEvent(e)) {
        switch (e.type) {
        case sf::Event::Closed:
            _window->close();
            _isRunning = false;
            break;

        default:
            Ic()->HandleEvent(e);
            break;
        }
    }
}

} // namespace REngine
