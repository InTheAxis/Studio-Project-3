#ifndef UNI_BLEND_H
#define UNI_BLEND_H

#include "Vector3.h"

class Blend
{
public:
	Vector3 pos;
	float radius, dura;
	float cutoff; //0 to 1
	bool enabled;
	
	double elapsedTime;

	Blend()
	{
		pos.Set(0, 0, 0);
		radius = 0.8f;
		dura = 1.f;
		enabled = false;

		elapsedTime = 0.0;
		cutoff = 0.8;
	}
};

#endif