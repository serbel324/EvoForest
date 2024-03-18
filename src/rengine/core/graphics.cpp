#include <rengine/core/graphics.h>
#include <library/vec2.h>
#include <library/ext_math.h>

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

namespace REngine {

Graphics::Graphics(std::shared_ptr<sf::RenderWindow> win, Vec2<int> ws)
{
    _window = win;
    _fillColor = Color(0, 0, 0, 255);
    _windowSize = ws;
    SetDefaultCamera();
}

Graphics::Graphics(std::shared_ptr<sf::RenderWindow> win, Vec2<int> ws, Camera::SPtr cam)
{
    _window = win;
    _fillColor = Color(0, 0, 0, 255);
    _windowSize = ws;
    _camera = cam;
}

void Graphics::DrawPoint(float x, float y)
{
    DrawPoint(Vec2<float>(x, y));
}

void Graphics::DrawPoint(Vec2<float> pos)
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(1, 1));
    rectangle.setPosition(pos.x, pos.y);

    rectangle.setFillColor(sf::Color(_fillColor.r, _fillColor.g, _fillColor.b, _fillColor.a));

    _window->draw(rectangle);
}

void Graphics::DrawCircle(float x, float y, float radius)
{
    DrawCircle(Vec2<float>(x, y), radius);
}
void Graphics::DrawCircle(Vec2<float> pos, float radius)
{
    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setPosition(pos.x - radius, pos.y - radius);

    circle.setFillColor(sf::Color(_fillColor.r, _fillColor.g, _fillColor.b, _fillColor.a));

    _window->draw(circle);
}

void Graphics::DrawBrokenLine(std::vector<Vec2<float>> t, Vec2<float> a, float s)
{
    for (size_t i = 1; i < t.size(); i++)
    {
        DrawLine(t[i - 1] * s + a, t[i] * s + a);
    }
}

void Graphics::DrawLine(Vec2<float> v1, Vec2<float> v2)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(v1.x, v1.y), sf::Color(_fillColor.r, _fillColor.g, _fillColor.b, _fillColor.a)),
        sf::Vertex(sf::Vector2f(v2.x, v2.y), sf::Color(_fillColor.r, _fillColor.g, _fillColor.b, _fillColor.a))
    };

    _window->draw(line, 2, sf::Lines);
}

void Graphics::DrawLine(float x1, float y1, float x2, float y2)
{
    DrawLine(Vec2<float>(x1, y1), Vec2<float>(x2, y2));
}


void Graphics::DrawRect(float x, float y, float W, float h)
{
    DrawRect(Vec2<float>(x, y), Vec2<float>(W, h));
}

void Graphics::DrawRect(Vec2<float> pos, Vec2<float> size)
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(size.x, size.y));
    rectangle.setPosition(pos.x, pos.y);

    rectangle.setFillColor(sf::Color(_fillColor.r, _fillColor.g, _fillColor.b, _fillColor.a));

    _window->draw(rectangle);
}

void Graphics::DrawTexture(sf::Texture& tex, float x, float y, float W, float h)
{
    DrawTexture(tex, Vec2<float>(x, y), Vec2<float>(W, h));
}

void Graphics::DrawTexture(sf::Texture& tex, Vec2<float> pos, Vec2<float> size)
{
    sf::Sprite sprite;
    sprite.setTexture(tex);

    sf::Vector2u s = tex.getSize();

    sprite.setPosition(pos.x - size.x / 2, pos.y - size.y / 2);
    sprite.setScale(size.x / s.x, size.y / s.y);
    _window->draw(sprite);
}

void Graphics::DrawTexture(sf::Texture& tex, Vec2<float> pos, Vec2<float> size, float a)
{
    sf::Sprite sprite;
    sprite.setTexture(tex);

    sf::Vector2u s = tex.getSize();

    sprite.setPosition(pos.x - size.x / 2, pos.y - size.y / 2);
    sprite.setScale(size.x / s.x, size.y / s.y);

    sprite.setOrigin(size.x / 2, size.y / 2);
    sprite.setRotation(ExtMath::ToDegrees(a));

    _window->draw(sprite);
}

void Graphics::Present()
{
    _window->display();
}

void Graphics::SetFillColor(float r, float g, float b, float a)
{
    _fillColor.r = r;
    _fillColor.g = g;
    _fillColor.a = b;
    _fillColor.a = a;
}

void Graphics::SetFillColor(Color col)
{
    _fillColor = col;
}

void Graphics::Fill()
{
    Camera::SPtr c = std::make_shared<Camera>();

    ApplyCamera(c);
    DrawRect(0, 0, _window->getSize().x, _window->getSize().y);
    ApplyCamera(_camera);
}

void Graphics::Clear()
{
    _window->clear();
}

void Graphics::FillText(const std::string& text, float x, float y, float size, sf::Font& font)
{
    sf::Text t;

    t.setFont(font);
    t.setString(text);
    t.setPosition(x, y);
    t.setCharacterSize(size);
    t.setFillColor(sf::Color(_fillColor.r, _fillColor.g, _fillColor.b, _fillColor.a));
    _window->draw(t);
}

void Graphics::ApplyCamera(Camera::SPtr cam)
{
    sf::View view;
    Vec2<float> p = cam->position - _windowSize / 2;
    Vec2<float> s = _windowSize * cam->scale;

    view.reset(sf::FloatRect(p.x, p.y, s.x, s.y));
    view.rotate(cam->angle);
    view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    _window->setView(view);
}

void Graphics::SetCamera(Camera::SPtr cam)
{
    _camera = cam;
}

void Graphics::SetDefaultCamera()
{
    _camera = std::make_shared<Camera>();
}

} // namespace REngine
