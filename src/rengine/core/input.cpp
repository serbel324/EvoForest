#include <rengine/core/input.h>

namespace REngine {

void InputController::HandleEvent(sf::Event event) {
    switch (event.type)
    {
    case sf::Event::KeyPressed:
        KeyPressed(event.key);
        if (keyPressedCallback) {
            keyPressedCallback(event.key.code);
        }
        break;
    case sf::Event::KeyReleased:
        KeyReleased(event.key);
        if (keyReleasedCallback) {
            keyReleasedCallback(event.key.code);
        }
        break;
    case sf::Event::MouseButtonPressed:
        MousePressed(event.mouseButton);
        break;
    case sf::Event::MouseButtonReleased:
        MouseReleased(event.mouseButton);
        break;
    case sf::Event::MouseMoved:
        MouseMoved(event.mouseMove);
        break;
    default:
        break;
    }
}

void InputController::KeyPressed(sf::Event::KeyEvent& event)
{
#define SET_KEY(code, var)      \
    case sf::Keyboard::code:    \
        var = true;             \
        break;

    switch (event.code) {
    SET_KEY(W, w)
    SET_KEY(A, a)
    SET_KEY(S, s)
    SET_KEY(D, d)
    SET_KEY(Space, space)
    default:
        break;
    }

#undef SET_KEY 
}

void InputController::KeyReleased(sf::Event::KeyEvent& event)
{
#define RESET_KEY(code, var)    \
    case sf::Keyboard::code:    \
        var = false;            \
        break;

    switch (event.code) {
    RESET_KEY(W, w)
    RESET_KEY(A, a)
    RESET_KEY(S, s)
    RESET_KEY(D, d)
    RESET_KEY(Space, space)
    default:
        break;
    }
#undef RESET_KEY 
}

void InputController::MouseMoved(sf::Event::MouseMoveEvent& event)
{
    mousePosition.Set(event.x, event.y);
}

void InputController::MousePressed(sf::Event::MouseButtonEvent& event)
{
    switch (event.button) {
    case sf::Mouse::Left:
        lmb = true;
        break;
    case sf::Mouse::Right:
        rmb = true;
        break;
    default:
        break;
    }
}

void InputController::MouseReleased(sf::Event::MouseButtonEvent& event)
{
    switch (event.button) {
    case sf::Mouse::Left:
        lmb = false;
        break;
    case sf::Mouse::Right:
        rmb = false;
        break;
    default:
        break;
    }
}

} // namespace REngine
