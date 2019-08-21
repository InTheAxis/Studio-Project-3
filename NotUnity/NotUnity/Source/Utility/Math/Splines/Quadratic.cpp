#include "Quadratic.h"

SplineQuadratic::SplineQuadratic() 
	: scale(1)
	, offset(0)
{
}

SplineQuadratic::~SplineQuadratic()
{
}

float SplineQuadratic::Fn(float x)
{
	return -x * x * scale + offset; //y = -xsq
}

Vector3 SplineQuadratic::Normal(float x)
{
	return Vector3(2 * x, 1, 0).Normalized(); // y' = -2x, -1 / -2x
}

void SplineQuadratic::SetScale(float scale)
{
	this->scale = scale;
}

void SplineQuadratic::SetOffset(float offset)
{
	this->offset = offset;
}
