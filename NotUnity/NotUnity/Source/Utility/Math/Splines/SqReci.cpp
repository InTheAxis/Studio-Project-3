#include "SqReci.h"

SplineSqReci::SplineSqReci() 
	: scale(1)
	, offset(0)
{
}

SplineSqReci::~SplineSqReci()
{
}

float SplineSqReci::Fn(float x)
{
	if (x == 0)
		return 10000;

	return (1 / (x * x)) * scale + offset;
}

Vector3 SplineSqReci::Normal(float x)
{
	return Vector3(-2, (x* x *x), 0).Normalized();
}

void SplineSqReci::SetScale(float scale)
{
	this->scale = scale;
}

void SplineSqReci::SetOffset(float offset)
{
	this->offset = offset;
}
