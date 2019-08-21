#ifndef SPLINE_QUADR_H
#define SPLINE_QUADR_H

#include <vector>

#include "../Vector3.h"
#include "../Spline.h"

class SplineQuadratic : public Spline
{
public:
	SplineQuadratic();
	~SplineQuadratic();

	virtual float Fn(float x);
	virtual Vector3 Normal(float x);
	virtual void SetScale(float scale);
	virtual void SetOffset(float offset);

private:
	float scale;
	float offset;
};

#endif
