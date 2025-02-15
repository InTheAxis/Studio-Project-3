#ifndef SPLINE_EXPO_H
#define SPLINE_EXPO_H

#include <vector>

#include "../Vector3.h"
#include "../Spline.h"

class SplineExpo : public Spline
{
public:
	SplineExpo();
	~SplineExpo();

	virtual float Fn(float x);
	virtual Vector3 Normal(float x);

	virtual void SetScale(float scale);
	virtual void SetOffset(float offset);

private:
	float scale;
	float offset;
};

#endif
