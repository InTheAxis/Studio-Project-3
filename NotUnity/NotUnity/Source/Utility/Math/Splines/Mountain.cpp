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

	if (relX < -3 || relX  > 3)
		return -2;

	return 0.333f * (-relX * relX) + 1;
}

void SplineMountain::SetOffset(float offset)
{
	this->offset = offset;
}

Vector3 SplineMountain::Normal(float x)
{
	return Vector3(-2, (x* x *x), 0).Normalized();
}