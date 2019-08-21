#include "Logarithm.h"

SplineLogarithm::SplineLogarithm() 
	: scale(1)
	, offset(0)
{
}

SplineLogarithm::~SplineLogarithm()
{
}

float SplineLogarithm::Fn(float x)
{
	if (x < 0) x = 0;
	return log10(x) * scale + offset;
}

Vector3 SplineLogarithm::Normal(float x)
{
	return Vector3(-x, 1,0).Normalized();
}

void SplineLogarithm::SetScale(float scale)
{
	this->scale = scale;
}

void SplineLogarithm::SetOffset(float offset)
{
	this->offset = offset;
}
