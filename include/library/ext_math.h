#pragma once

#include <cstdlib>
#include <random>
#include <cmath>
#include <ctime>
#include <climits>

#include <library/vec2.h>

namespace ExtMath {

constexpr long double PI = 3.14159265359;
constexpr long double EPS = 1e-9;

double ToRadian(double a);

double ToDegrees(double a);

float WeightedAverage(float a1, float p1, float a2, float p2);

float Hypot(float a1, float a2);

float Hypot(float x1, float y1, float x2, float y2);

float Hypot(Vec2<float> p1, Vec2<float> p2);

double Sigmoid(float x, float a);

int Sign(double a);

double RandomDouble(double a, double b);

int RandomInt(int a, int b);

double Interpolate(double a0, double a1, double p);

double Smootherstep(double a0, double a1, double p);

double InterpolateSquare(double p00, double p10, double p01, double p11, Vec2<double> p);

double SmootherstepSquare(double p00, double p10, double p01, double p11, Vec2<double> p);

double ModuleStepFunction(double x);

}
