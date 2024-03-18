#pragma once
#include <library/vec2.h>
#include <rengine/core/color.h>
#include <rengine/core/camera.h>

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

namespace REngine
{

/* Contains functions for drawing */
class Graphics
{
public:
    using SPtr = std::shared_ptr<Graphics>;

public:
    /* requires RenderWindow pointer, Window's dimensions and Camera object pointer (optionally) */
    Graphics(std::shared_ptr<sf::RenderWindow> win, Vec2<int> ws);
    Graphics(std::shared_ptr<sf::RenderWindow> win, Vec2<int> ws, Camera::SPtr cam);

    /* draw a point with given coordinates */
    void DrawPoint(Vec2<float> pos);
    void DrawPoint(float x, float y);

    /* draw a point with given coordinates */
    void DrawCircle(float x, float y, float radius);
    void DrawCircle(Vec2<float> pos, float radius);

    /* draw broken line fith given verticies */
    void DrawBrokenLine(std::vector<Vec2<float>> t, Vec2<float> a, float s);

    /* draw straight line from one point to another */
    void DrawLine(Vec2<float> v1, Vec2<float> v2);
    void DrawLine(float x1, float y1, float x2, float y2);

    /* draw rectangle with given upper left Angle coordinates, width and height */
    void DrawRect(float x, float y, float w, float h);
    void DrawRect(Vec2<float> pos, Vec2<float> size);

    /* draw texture with given upper left Angle coordinates, width, height and rotation (rotation center is in the middle) */
    void DrawTexture(sf::Texture& tex, float x, float y, float w, float h);
    void DrawTexture(sf::Texture& tex, Vec2<float> pos, Vec2<float> size);
    void DrawTexture(sf::Texture& tex, Vec2<float> pos, Vec2<float> size, float a);

    /* Set color for drawing primitives */
    void SetFillColor(float r, float g, float b, float a);
    void SetFillColor(Color col);

    /* Fill the screen with the same color */
    void Fill();

    /* Clear all */
    void Clear();

    /* draw text with given coordinates, size and font */
    void FillText(const std::string& text, float x, float y, float size, sf::Font& font);

    /* auxilary method */
    void ApplyCamera(Camera::SPtr cam);

    /* Set camera */
    void SetCamera(Camera::SPtr cam);

    /* Set default camera parameters */
    void SetDefaultCamera();

    /* update the window */
    void Present();

private:
    std::shared_ptr<sf::RenderWindow> _window;
    Color _fillColor;

    Camera::SPtr _camera;
    Vec2<int> _windowSize;
};

}