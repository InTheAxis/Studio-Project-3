#include "Mountain.h"

SplineMountain::SplineMountain()
	: offset(0)
{
}

SplineMountain::~SplineMountain()
{
}

float SplineMountain::Fn(float x)
{
	float relX = x + offset;

	if (relX < -6 || relX  > 6)
		return -5;

	return 0.19f * (-relX * relX) + 2.2f;
}

Vector3 SplineMountain::Normal(float x)
{
	return Vector3(-2, (x* x *x), 0).Normalized();
}