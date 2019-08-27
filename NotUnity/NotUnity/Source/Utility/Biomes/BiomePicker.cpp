#include "BiomePicker.h"

#include "../Math/MyMath.h"
#include "Biomes.h"

Biome* BiomePicker::Pick()
{
	//TODO improve biome logic

	switch (Math::RandIntMinMax(0, 12))
	{
	case 0:
	case 6:
	case 9:
	case 10:
		return new BiomeMt;
	case 1:
	case 7:
		return new BiomeSnow;
	case 2:
	case 8:
		return new BiomeDes;
	case 3:
	case 11:
	case 12:
		return new BiomeMtFlat;
	case 4:
		return new BiomeSnowFlat;
	case 5:
		return new BiomeDesFlat;
	}
	return nullptr;
}