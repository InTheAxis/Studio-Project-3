#include "Cubic.h"

SplineCubic::SplineCubic()
{
}

SplineCubic::~SplineCubic()
{
}

float SplineCubic::Fn(float x)
{
	return x * x * x * 2; //y = -xcube
}

Vector3 SplineCubic::Normal(float x)
{
	return Vector3(-3 * 2 * (x* x), 1, 0).Normalized(); // y' = 3x^2
}