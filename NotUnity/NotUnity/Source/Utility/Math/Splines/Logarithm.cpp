#include "Logarithm.h"

SplineLogarithm::SplineLogarithm()
{
}

SplineLogarithm::~SplineLogarithm()
{
}

float SplineLogarithm::Fn(float x)
{
	if (x < 0) x = 0;
	return log10(x);
}

Vector3 SplineLogarithm::Normal(float x)
{
	return Vector3(-x, 1,0).Normalized();
}