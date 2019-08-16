#ifndef RECT_H
#define RECT_H

#include "Vector3.h"

class Rect2D 
{
public:
	Rect2D();
	Rect2D(Vector3 min, Vector3 max);
	~Rect2D();

	bool isWithin(Vector3 pos, Vector3* penetration = nullptr);

	Vector3 min, max;
};

#endif
