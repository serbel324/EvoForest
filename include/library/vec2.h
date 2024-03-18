#pragma once
#include <cmath>
#include <iostream>

/* T must be algebraic field */
template <typename T, class F = float>
struct Vec2
{
    Vec2();
    template<typename T1>
    Vec2(T1 x, T1 y);
    template<typename T1, typename F1>
    Vec2(const Vec2<T1, F1>& other);
    template<typename T1, typename F1>
    Vec2(Vec2<T1, F1>&& other);
    
    /* comparison without eps, be careful with floating types */
    template<typename T1, typename F1>
    bool operator==(const Vec2<T1, F1>& other) const;
    template<typename T1, typename F1>
    bool operator!=(const Vec2<T1, F1>& other) const;

    /* true if this' and other's coordinates differerence is no more than eps */
    template<typename T1, typename F1>
    bool compare_with_eps(const Vec2<T1, F1>& other) const;

    template<typename T1, typename F1>
    Vec2& operator=(const Vec2<T1, F1>& other);
    template<typename T1, typename F1>
    Vec2& operator=(Vec2<T1, F1>&& other);

    template<typename T1, typename T2>
    void Set(T1 x, T2 y);

    Vec2 operator-() const;

    template <typename T1, typename F1>
    Vec2& operator+=(Vec2<T1, F1> r);
    template <typename T1, typename F1>
    Vec2& operator-=(Vec2<T1, F1> r);

    template <typename T1>
    Vec2& operator*=(T1 r);
    template <typename T1>
    Vec2& operator/=(T1 r);

    template <typename T1, typename F1>
    Vec2& operator*=(Vec2<T1, F1> r);
    template <typename T1, typename F1>
    Vec2& operator/=(Vec2<T1, F1> r);

    template <typename T1, typename F1>
    Vec2 operator+(Vec2<T1, F1> r) const;
    template <typename T1, typename F1>
    Vec2 operator-(Vec2<T1, F1> r) const;

    template <typename T1>
    Vec2 operator*(T1 r) const;
    template <typename T1>
    Vec2 operator/(T1 r) const;

    template <typename T1, typename F1>
    Vec2 operator*(Vec2<T1, F1> r) const;
    template <typename T1, typename F1>
    Vec2 operator/(Vec2<T1, F1> r) const;

    F magnitude() const;
    Vec2 normalized() const;

    template <typename F1>
    Vec2 rotated(F1 a) const;

    T x;
    T y;

    static constexpr long double EPS = 1e-9;
};

using Vec2i = Vec2<int>;
using Vec2u = Vec2<uint32_t>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

 /* returns scalar product */
template <typename T, typename F>
T dot_prod(const Vec2<T, F>& l, const Vec2<T, F>& r);

/* returns vector product = |x|*|y|*sin(angle(x,y)) */
template <typename T, typename F>
T cross_prod(const Vec2<T, F>& l, const Vec2<T, F>& r);

/* returns distance between point a and b */
template <typename T, typename F>
F dist(const Vec2<T, F>& a, const Vec2<T, F>& b);

/* returns angle between vectors a and b in range[-pi, pi] */
template <typename T, typename F>
F angle(const Vec2<T, F>& a, const Vec2<T, F>& b);

/* returns true if a and b are collinear */
template <typename T, typename F>
bool check_parallel(const Vec2<T, F>& a, const Vec2<T, F>& b);


/* ------------------ I/O operators ------------------ */
template <typename T, typename F>
std::istream& operator>>(std::istream& in, Vec2<T, F>& v);

template <typename T, typename F>
std::ostream& operator<<(std::ostream& out, const Vec2<T, F>& v);

/* =========================================================================== */
/* =============================== DEFINITIONS =============================== */
/* =========================================================================== */

template <typename T, typename F>
Vec2<T, F>::Vec2()
    : x(0), y(0)
{}

template <typename T, typename F>
template<typename T1>
Vec2<T, F>::Vec2(T1 x, T1 y)
    : x(x), y(y)
{}

template<typename T1, typename F1>
template<typename T2, typename F2>
Vec2<T1, F1>::Vec2(const Vec2<T2, F2>& other)
    : x(other.x), y(other.y)
{}

template<typename T1, typename F1>
template<typename T2, typename F2>
Vec2<T1, F1>::Vec2(Vec2<T2, F2>&& other)
    : x(other.x), y(other.y)
{
    other.x = 0;
    other.y = 0;
}

template<typename T1, typename F1>
template<typename T2, typename F2>
bool Vec2<T1, F1>::operator==(const Vec2<T2, F2>& other) const
{
    return (x == other.x && y == other.y);
}

template<typename T1, typename F1>
template<typename T2, typename F2>
bool Vec2<T1, F1>::operator!=(const Vec2<T2, F2>& other) const
{
    return (x != other.x || y != other.y);
}

template<typename T1, typename F1>
template<typename T2, typename F2>
bool Vec2<T1, F1>::compare_with_eps(const Vec2<T2, F2>& other) const
{
    return (std::abs(x - other.x) < EPS && std::abs(y - other.y) < EPS);
}

template<typename T1, typename F1>
template<typename T2, typename F2>
Vec2<T1, F1>& Vec2<T1, F1>::operator=(const Vec2<T2, F2>& other)
{
    x = other.x;
    y = other.y;

    return *this;
}

template<typename T1, typename F1>
template<typename T2, typename F2>
Vec2<T1, F1>& Vec2<T1, F1>::operator=(Vec2<T2, F2>&& other)
{
    x = other.x;
    y = other.y;

    return *this;
}

template<typename T, typename F>
template<typename T1, typename T2>
void Vec2<T, F>::Set(T1 x, T2 y)
{
    this->x = x;
    this->y = y;
}

template<typename T, typename F>
Vec2<T, F> Vec2<T, F>::operator-() const
{
    return Vec2(-x, -y);
}

template<typename T1, typename F1>
template<typename T2, typename F2>
Vec2<T1, F1>& Vec2<T1, F1>::operator+=(Vec2<T2, F2> r)
{
    x += r.x;
    y += r.y;
    return *this;
}

template<typename T1, typename F1>
template<typename T2, typename F2>
Vec2<T1, F1>& Vec2<T1, F1>::operator-=(Vec2<T2, F2> r)
{
    return *this += -r;
}

template<typename T1, typename F1>
template<typename T2, typename F2>
Vec2<T1, F1>& Vec2<T1, F1>::operator*=(Vec2<T2, F2> r)
{
    x *= r.x;
    y *= r.y;
    return *this;
}

template<typename T1, typename F1>
template<typename T2, typename F2>
Vec2<T1, F1>& Vec2<T1, F1>::operator/=(Vec2<T2, F2> r)
{
    x /= r.x;
    y /= r.y;
    return *this;
}

template<typename T1, typename F1>
template<typename T2, typename F2>
Vec2<T1, F1> Vec2<T1, F1>::operator+(Vec2<T2, F2> r) const
{
    Vec2 v(*this);
    return v += r;
}

template<typename T1, typename F1>
template<typename T2, typename F2>
Vec2<T1, F1> Vec2<T1, F1>::operator-(Vec2<T2, F2> r) const
{
    Vec2 v(*this);
    return v -= r;
}

template<typename T1, typename F1>
template<typename T2, typename F2>
Vec2<T1, F1> Vec2<T1, F1>::operator*(Vec2<T2, F2> r) const
{
    Vec2 v(*this);
    return v *= r;
}

template<typename T1, typename F1>
template<typename T2, typename F2>
Vec2<T1, F1> Vec2<T1, F1>::operator/(Vec2<T2, F2> r) const
{
    Vec2 v(*this);
    return v /= r;
}

template<typename T, typename F>
F Vec2<T, F>::magnitude() const
{
    return std::sqrt(x * x + y * y);
}

template<typename T, typename F>
Vec2<T, F> Vec2<T, F>::normalized() const
{
    F mag = magnitude();
    if (mag < EPS)
    {
        return *this;
    }
    return Vec2(x / mag, y / mag);
}

template<typename T, typename F>
template<typename F1>
inline Vec2<T, F> Vec2<T, F>::rotated(F1 a) const
{
    return Vec2(
        x * std::cos(a) - y * std::sin(a),
        x * std::sin(a) + y * std::cos(a));
}

template<typename T, typename F>
template<typename T1>
Vec2<T, F>& Vec2<T, F>::operator*=(T1 r)
{
    x *= r;
    y *= r;
    return *this;
}

template<typename T, typename F>
template<typename T1>
Vec2<T, F>& Vec2<T, F>::operator/=(T1 r)
{
    return *this *= 1 / r;
}

template<typename T, typename F>
template<typename T1>
Vec2<T, F> Vec2<T, F>::operator*(T1 r) const
{
    Vec2 v(*this);
    return v *= r;
}


template<typename T, typename F>
template<typename T1>
Vec2<T, F> Vec2<T, F>::operator/(T1 r) const
{
    Vec2 v(*this);
    return v /= r;
}

template<typename T, typename F>
std::istream& operator>>(std::istream& in, Vec2<T, F>& v)
{
    in >> v.x >> v.y;
    return in;
}

template<typename T, typename F>
std::ostream& operator<<(std::ostream& out, const Vec2<T, F>& v)
{
    out << v.x << ' ' << v.y;
    return out;
}

template<typename T, typename F>
T dot_prod(const Vec2<T, F>& l, const Vec2<T, F>& r)
{
    return l.x * r.x + l.y * r.y;
}

template<typename T, typename F>
T cross_prod(const Vec2<T, F>& l, const Vec2<T, F>& r)
{
    return l.x * r.y - l.y * r.x;
}

template<typename T, typename F>
F dist(const Vec2<T, F>& a, const Vec2<T, F>& b)
{
    return (a - b).abs();
}

template<typename T, typename F>
F angle(const Vec2<T, F>& a, const Vec2<T, F>& b)
{
    return std::atan2(cross_prod(a, b), dot_prod(a, b));
}

template<typename T, typename F>
bool check_parallel(const Vec2<T, F>& a, const Vec2<T, F>& b)
{
    double aabs = a.abs();
    double babs = b.abs();

    if (aabs < Vec2<T, F>::EPS || babs < Vec2<T, F>::EPS)
    {
        return true;
    }

    return (std::abs(cross_prod(a, b) / aabs / babs) < Vec2<T, F>::EPS);
}
