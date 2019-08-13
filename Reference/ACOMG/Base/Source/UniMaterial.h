#ifndef UNI_MATERIAL_H
#define UNI_MATERIAL_H

#include "Vector3.h"

class Material
{
public:
	Vector3 kAmbient;
	Vector3 kDiffuse;
	Vector3 kSpecular;
	float kShininess;

	Material()
	{
		//some default values
		kAmbient.Set(0.1f, 0.1f, 0.1f);		
		kDiffuse.Set(0.1f, 0.1f, 0.1f);
		kSpecular.Set(0.2f, 0.2f, 0.3f);
		kShininess = 2.f;
	}
};

#endif