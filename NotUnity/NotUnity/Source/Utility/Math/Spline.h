#ifndef SPLINE_H
#define SPLINE_H

#include <vector>

#include "Vector3.h"

class Spline 
{
public:
	Spline();
	~Spline();

	virtual float Fn(float x);
	virtual Vector3 Normal(float x);

	std::vector<Vector3> GetSamplePts();
};

#endif
