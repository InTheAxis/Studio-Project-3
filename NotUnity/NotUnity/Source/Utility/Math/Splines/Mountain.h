#ifndef SPLINE_MT_H
#define SPLINE_MT_H

#include <vector>

#include "../Vector3.h"
#include "../Spline.h"

class SplineMountain : public Spline
{
public:
	SplineMountain();
	~SplineMountain();

	virtual float Fn(float x);
	virtual void SetOffset(float offset);
	virtual Vector3 Normal(float x);

private:
	float offset;
};

#endif
