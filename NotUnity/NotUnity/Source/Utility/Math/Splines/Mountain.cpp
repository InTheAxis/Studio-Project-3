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

	if (relX < -1 || relX  > 1)
		return -3;

	return -relX * relX - 2;
}

Vector3 SplineMountain::Normal(float x)
{
	return Vector3(-2, (x* x *x), 0).Normalized();
}