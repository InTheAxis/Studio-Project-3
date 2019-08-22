#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../Resource.h"
#include "../Manager/MgrGraphics.h"

class Material
{
public:
	Material(std::string name)
		:name(name)
	{		
		albedo.Set(1, 1, 1, 1);
		for (int i = 0; i < NUM_MAPS; ++i)
			maps[i] = 0;

		if (!MgrGraphics::Instance()->GetCachedMaterial(name))
			MgrGraphics::Instance()->CacheMaterial(this);
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
		COLOR8,
		COLOR9,
		COLOR10,
		COLOR11,
		COLOR12,
		COLOR13,
		DIFFUSE,
		SPECULAR,
		NORMAL,
		NUM_MAPS
	};

	std::string name;
	Vector4 albedo;
	unsigned maps[NUM_MAPS];
};

#endif