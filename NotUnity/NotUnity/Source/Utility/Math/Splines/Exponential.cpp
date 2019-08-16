#include "Exponential.h"

SplineExpo::SplineExpo()
{
}

SplineExpo::~SplineExpo()
{
}

float SplineExpo::Fn(float x)
{
	return pow(2.71828182846f, x);
}

Vector3 SplineExpo::Normal(float x)
{
	return Vector3(pow(2.71828182846f, x), 1, 0).Normalized();
}