#pragma once

#include <cstdint>
#include <cmath>
#include <random>

#include <library/ext_math.h>

template <int64_t Numerator, uint64_t Denominator, typename T = double>
struct SimpleFraction {
    static_assert(Denominator > 0);
    constexpr static double value = static_cast<T>(Numerator) / Denominator; 
};

template <int64_t I, typename T = double>
struct Int {
    constexpr static double value = static_cast<T>(I); 
};

struct Pi {
    constexpr static double value = ExtMath::PI; 
};

struct TransformIdentical {
    static double Transform(double x) {
        return x;
    }
};

template<typename TransformX, typename Y>
struct TransformMul {
    static double Transform(double x) {
        return TransformX::Transform(x) * Y::value;
    }
};

template<typename TransformX, typename Y>
struct TransformSum {
    static double Transform(double x) {
        return TransformX::Transform(x) + Y::value;
    }
};

template<typename L = Int<1>, typename x0 = Int<0>, typename k = Int<1>>
struct TransformLogistic {
    static double Transform(double x) {
        return L::value / (1 + std::exp(k::value * (x - x0::value)));
    }
};

template<typename Min, typename Max, typename F>
struct TransformBounded {
    static double Transform(double x) {
        return std::max(Min::value, std::min(Max::value, F::Transform(x)));
    }
};
