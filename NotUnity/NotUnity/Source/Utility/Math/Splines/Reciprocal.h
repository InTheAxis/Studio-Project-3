#ifndef SPLINE_RECIPROCAL_H
#define SPLINE_RECIPROCAL_H

#include <vector>

#include "../Vector3.h"
#include "../Spline.h"

class SplineReciprocal : public Spline
{
public:
	SplineReciprocal();
	~SplineReciprocal();

	virtual float Fn(float x);
	virtual Vector3 Normal(float x);
	virtual void SetScale(float scale);
	virtual void SetOffset(float offset);

private:
	float scale;
	float offset;
};

#endif
