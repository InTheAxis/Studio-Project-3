#include "Cubic.h"

SplineCubic::SplineCubic() 
	: scale(1)
	, offset(0)
{
}

SplineCubic::~SplineCubic()
{
}

float SplineCubic::Fn(float x)
{
	return x * x * x * 2 * scale + offset; //y = -xcube
}

Vector3 SplineCubic::Normal(float x)
{
	return Vector3(-3 * 2 * (x* x), 1, 0).Normalized(); // y' = 3x^2
}

void SplineCubic::SetScale(float scale)
{
	this->scale = scale;
}

void SplineCubic::SetOffset(float offset)
{
	this->offset = offset;
}
