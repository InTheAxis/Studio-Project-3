#ifndef SPLINE_H
#define SPLINE_H

#include <vector>

#include "Vector3.h"

class Spline 
{
public:
	Spline();
	virtual ~Spline();

	virtual float Fn(float x);
	virtual Vector3 Normal(float x);
	virtual void SetOffset(float offset);

	std::vector<Vector3> GetSamplePts();

private:
	float offset;

};

#endif
