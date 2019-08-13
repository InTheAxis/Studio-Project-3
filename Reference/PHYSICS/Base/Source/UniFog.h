#ifndef UNI_FOG_H
#define UNI_FOG_H

#include "Vector4.h"

class Fog
{
public:
	enum FOG_TYPE
	{		
		FOG_NULL = -1,
		FOG_LINEAR = 0,
		FOG_EXPO,
		FOG_EXPO_SQ,
	};
	bool enabled;
	FOG_TYPE type;
	Vector4 color;
	float start, end, density;

	Fog()
	{
		type = FOG_LINEAR;
		color.Set(0.5f, 0.5f, 0.5f);
		start = 10;
		end = 1000;
		density = 0.0005f;
		enabled = false;
	}
};

#endif