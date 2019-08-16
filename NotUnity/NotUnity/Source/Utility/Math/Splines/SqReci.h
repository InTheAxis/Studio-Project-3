#ifndef SPLINE_SQRECI_H
#define SPLINE_SQRECI_H

#include <vector>

#include "../Vector3.h"
#include "../Spline.h"

class SplineSqReci : public Spline
{
public:
	SplineSqReci();
	~SplineSqReci();

	virtual float Fn(float x);
	virtual Vector3 Normal(float x);
};

#endif
