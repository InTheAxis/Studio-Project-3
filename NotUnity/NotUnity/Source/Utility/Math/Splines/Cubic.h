#ifndef SPLINE_CUBIC_H
#define SPLINE_CUBIC_H

#include <vector>

#include "../Vector3.h"
#include "../Spline.h"

class SplineCubic : public Spline
{
public:
	SplineCubic();
	~SplineCubic();

	virtual float Fn(float x);
	virtual Vector3 Normal(float x);

	virtual void SetScale(float scale);
	virtual void SetOffset(float offset);

private:
	float scale;
	float offset;
};

#endif
