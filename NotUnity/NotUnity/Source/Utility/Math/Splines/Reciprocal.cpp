#include "Reciprocal.h"

SplineReciprocal::SplineReciprocal()
{
}

SplineReciprocal::~SplineReciprocal()
{
}

float SplineReciprocal::Fn(float x)
{
	return -1 / x;
}

Vector3 SplineReciprocal::Normal(float x)
{
	return Vector3(-1, (x* x), 0).Normalized();
}