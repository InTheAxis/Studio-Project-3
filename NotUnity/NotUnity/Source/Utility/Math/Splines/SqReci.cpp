#include "SqReci.h"

SplineSqReci::SplineSqReci()
{
}

SplineSqReci::~SplineSqReci()
{
}

float SplineSqReci::Fn(float x)
{
	if (x == 0)
		return 10000;

	return 1 / (x * x);
}

Vector3 SplineSqReci::Normal(float x)
{
	return Vector3(-2, (x* x *x), 0).Normalized();
}