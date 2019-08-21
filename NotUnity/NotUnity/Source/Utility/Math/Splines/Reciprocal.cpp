#include "Reciprocal.h"

SplineReciprocal::SplineReciprocal() : scale(1), offset(0)
{
}

SplineReciprocal::~SplineReciprocal()
{
}

float SplineReciprocal::Fn(float x)
{
	return (-1 / x) * scale + offset;
}

Vector3 SplineReciprocal::Normal(float x)
{
	return Vector3(-1, (x* x), 0).Normalized();
}

void SplineReciprocal::SetScale(float scale)
{
	this->scale = scale;
}

void SplineReciprocal::SetOffset(float offset)
{
	this->offset = offset;
}
