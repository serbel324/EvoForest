#pragma once
#include <library/vec2.h>
#include <memory>

namespace REngine {

class Camera
{
public:
    using SPtr = std::shared_ptr<Camera>;

public:
    Camera();
    Camera(float x, float y, float a = 0, float s = 1);
    Camera(Vec2<float> pos, float a = 0, float s = 1);

    Vec2<float> position;
    float angle;
    float scale;

};

} // namespace REngine
