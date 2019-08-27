#include "BiomeBase.h"

Biome::Biome()
	: spriteIdx(0)
	, spline(nullptr)
	, interactiveType(INTER::NONE)
{
}

Biome::~Biome()
{
	if (spline)
	{
		delete spline;
		spline = nullptr;
	}
}

int Biome::GetSpriteIdx()
{
	return spriteIdx;
}

Spline * Biome::GetSpline()
{
	return spline;
}

INTER Biome::GetInteractiveType()
{
	return interactiveType;
}
