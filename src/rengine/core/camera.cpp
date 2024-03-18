#include <rengine/core/camera.h>

namespace REngine {

Camera::Camera()
{
    position = Vec2<float>(0, 0);
    angle = 0;
    scale = 1;
}

Camera::Camera(float x, float y, float a, float s)
{
    position = Vec2<float>(x, y);
    angle = a;
    scale = s;
}

Camera::Camera(Vec2<float> pos, float a, float s)
{
    position = Vec2<float>(pos);
    angle = a;
    scale = s;
}

} // namespace REngine
