#include "Spline.h"

Spline::Spline()
{
}

Spline::~Spline()
{
}

float Spline::Fn(float x)
{
	return x;
}

Vector3 Spline::Normal(float x)
{
	return Vector3(-1, 1, 0).Normalized();
}

std::vector<Vector3> Spline::GetSamplePts()
{
	std::vector<Vector3> ret;
	Vector3 temp;
	for (float x = -0.5f; x <= 0.5f; x += 0.1f)
	{
		temp.Set(x, Fn(x), 0);
		ret.emplace_back(temp);
	}

	//temp = Normal(0.5f);
	//ret.emplace_back(temp);
	return ret;
}
