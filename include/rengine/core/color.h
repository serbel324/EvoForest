#pragma once

namespace REngine {

class Color
{
public:
    Color(int r, int g, int b, int a = 255);
    /* r, g, b are set to the same value v */
    Color(int v = 0);

    Color operator+ (const Color& right) const;
    Color operator- (const Color& right) const;
    Color operator+ (const int right) const;
    Color operator- (const int right) const;
    Color operator* (const float right) const;

public:
    int r;
    int g;
    int b;
    int a;

public:
    static Color GetRandom();

public:
    const static Color RED;
    const static Color GREEN;
    const static Color BLUE;
    const static Color WHITE;
    const static Color BLACK;
};

} // namespace REngine
