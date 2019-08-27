#include "BiomePicker.h"

#include "../Math/MyMath.h"
#include "Biomes.h"

Biome* BiomePicker::Pick()
{
	//TODO improve biome logic

	switch (Math::RandIntMinMax(0, 2))
	{
	case 0:
		return new BiomeMt;
	case 1:
		return new BiomeSnow;
	case 2:
		return new BiomeDes;
	}
	return nullptr;
}