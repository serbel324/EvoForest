#include <rengine/core/color.h>
#include <library/ext_math.h>

#include <algorithm>

namespace REngine {

Color::Color(int r, int g, int b, int a)
    : r(std::max(std::min(255, r), 0))
    , g(std::max(std::min(255, g), 0))
    , b(std::max(std::min(255, b), 0))
    , a(std::max(std::min(255, a), 0))
{}

Color::Color(int v)
    : r(std::max(std::min(255, v), 0))
    , g(std::max(std::min(255, g), 0))
    , b(std::max(std::min(255, g), 0))
    , a(255)
{}

Color Color::operator+(const Color& right) const
{
    return Color(r + right.r, g + right.g, b + right.b, a);
}

Color Color::operator+(const int right) const
{
    return Color(r + right, g + right, b + right, a);
}

Color Color::operator-(const Color& right) const
{
    return Color(r - right.r, g - right.g, b - right.b, a);
}

Color Color::operator-(const int right) const
{
    return Color(r - right, g - right, b - right, a);
}

Color Color::operator*(const float right) const
{
    return Color(r * right, g * right, b * right, a);
}

Color Color::GetRandom()
{
    return Color(
        ExtMath::RandomInt(0, 255), 
        ExtMath::RandomInt(0, 255), 
        ExtMath::RandomInt(0, 255)
    );
}

const Color Color::RED = Color(255, 0, 0);
const Color Color::GREEN = Color(0, 255, 0);
const Color Color::BLUE = Color(0, 0, 255);
const Color Color::WHITE = Color(255, 255, 255);
const Color Color::BLACK = Color(0, 0, 0);

} // namespace REngine
