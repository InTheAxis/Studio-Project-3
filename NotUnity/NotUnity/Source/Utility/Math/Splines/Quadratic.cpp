#include "Quadratic.h"

SplineQuadratic::SplineQuadratic()
{
}

SplineQuadratic::~SplineQuadratic()
{
}

float SplineQuadratic::Fn(float x)
{
	return -x * x; //y = -xsq
}

Vector3 SplineQuadratic::Normal(float x)
{
	return Vector3(2 * x, 1, 0).Normalized(); // y' = -2x, -1 / -2x
}