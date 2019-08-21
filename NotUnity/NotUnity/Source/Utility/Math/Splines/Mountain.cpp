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

	if (relX < -5.5f || relX  > 5.5f)
		return -3;

	return 0.2f * (-relX * relX) + 3.05f;
}

void SplineMountain::SetOffset(float offset)
{
	this->offset = offset;
}

Vector3 SplineMountain::Normal(float x)
{
	return Vector3(-2, (x* x *x), 0).Normalized();
}