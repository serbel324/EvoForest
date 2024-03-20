#include <cstdlib>
#include <random>
#include <cmath>
#include <ctime>
#include <climits>

#include <library/ext_math.h>

namespace ExtMath {

double ToRadian(double a)
{
    return (a * PI / 180);
}

double ToDegrees(double a)
{
    return (a * 180 / PI);
}

float WeightedAverage(float a1, float p1, float a2, float p2)
{
    return (a1 * p1 + a2 * p2) / (p1 + p2);
}

float Hypot(float a1, float a2)
{
    return(sqrt(a1 * a1 + a2 * a2));
}

float Hypot(float x1, float y1, float x2, float y2)
{
    return(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}

float Hypot(Vec2<float> p1, Vec2<float> p2) {
    return(sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)));
}

double Sigmoid(float x, float a)
{
    return 1 / (1 + exp(-x * a));
}

int Sign(double a)
{
    if (a == 0)    return 0;
    if (a > 0) return 1;
    return -1;
}

double RandomDouble(double a, double b)
{
    if (b <= a) 
    {
        return 0;
    }

    static std::mt19937_64 randomizer(std::time(0));
    long double base = randomizer(); /* in range [0; ULLONG_MAX) */
    long double maxr = ULLONG_MAX;
    long double normilized = base / maxr; /* in range [0; ULLONG_MAX) */
    return normilized * (b - a) + a;
}

int RandomInt(int a, int b)
{
    if (b <= a) 
    {
        return 0;
    }

    static std::mt19937_64 randomizer(std::time(0));
    unsigned long long base = randomizer(); /* in range [0; ULLONG_MAX) */
    long long normilized = base % (b - a);
    return normilized + a;
}

double Interpolate(double a0, double a1, double p)
{
	return a1 * p + (1 - p) * a0;
}

double Smootherstep(double a0, double a1, double p)
{
	return (a1 - a0) * ((p * (p * 6.0 - 15.0) + 10.0) * p * p * p) + a0;
}

double InterpolateSquare(double p00, double p10, double p01, double p11, Vec2<double> p)
{
	return Interpolate(
		Interpolate(p00, p10, p.x),
		Interpolate(p01, p11, p.x),
		p.y
	);
}

double SmootherstepSquare(double p00, double p10, double p01, double p11, Vec2<double> p)
{
	return Smootherstep(
		Smootherstep(p00, p10, p.x),
		Smootherstep(p01, p11, p.x),
		p.y
	);
}

double ModuleStepFunction(double x) {
    return x / (1 + std::abs(x));
}

}
