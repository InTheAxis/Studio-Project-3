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
		kAmbient.Set(0.4f, 0.4f, 0.4f);		
		kDiffuse.Set(0.4f, 0.4f, 0.4f);
		kSpecular.Set(0.4f, 0.4f, 0.4f);
		kShininess = 4.f;
	}
};

#endif