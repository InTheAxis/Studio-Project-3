#include "Exponential.h"

SplineExpo::SplineExpo() 
	: scale(1)
	, offset(0)
{
}

SplineExpo::~SplineExpo()
{
}

float SplineExpo::Fn(float x)
{
	return pow(2.71828182846f, x) * scale + offset;
}

Vector3 SplineExpo::Normal(float x)
{
	return Vector3(pow(2.71828182846f, x), 1, 0).Normalized();
}

void SplineExpo::SetScale(float scale)
{
	this->scale = scale;
}

void SplineExpo::SetOffset(float offset)
{
	this->offset = offset;
}
