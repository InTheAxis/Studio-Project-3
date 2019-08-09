#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../Resource.h"

class Material
{
public:
	Material() 
	{
		albedo.Set(1, 1, 1, 1);
		maps[COLOR0] = Resource::LoadTGA("Tga/placeholder.tga"); 
		for (int i = 1; i < NUM_MAPS; ++i)
			maps[i] = 0;
	}
	~Material() {}
	enum MAPS
	{
		COLOR0 = 0,
		COLOR1,
		COLOR2,
		COLOR3,
		COLOR4,
		COLOR5,
		COLOR6,
		COLOR7,
		DIFFUSE = 8,
		SPECULAR,
		NORMAL,
		NUM_MAPS
	};
	Vector4 albedo;
	unsigned maps[NUM_MAPS];
};

#endif