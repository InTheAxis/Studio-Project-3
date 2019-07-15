#ifndef UNI_LIGHT_H
#define UNI_LIGHT_H

#include "Vector3.h"
#include "Vector4.h"

class Light
{
public:
	enum LIGHT_TYPE
	{		
		LIGHT_NULL = -1,
		LIGHT_POINT = 0,
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,
	};
	bool enabled;
	LIGHT_TYPE type;
	Vector3 position;
	Vector4 color;
	float power;
	float kC;
	float kL;
	float kQ;
	Vector3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;

	Light()
	{
		enabled = false;
		type = LIGHT_POINT;
		position.Set(0, 10, 0);
		color.Set(1, 1, 1, 1);
		power = 1.f;
		kC = 1.f;
		kL = 0.014f;
		kQ = 0.0007f;
		spotDirection.Set(0.f, 1.f, 0.f);
		cosInner = 0.8;
		cosCutoff = 0.8;
		exponent = 3.f;
	}
};

#endif