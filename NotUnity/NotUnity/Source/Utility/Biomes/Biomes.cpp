#include "Biomes.h"

#include "../Math/Splines/Mountain.h"
#include "../../Node/GameObj.h"

BiomeMt::BiomeMt()
{
	spriteIdx = 0;
	spline = new SplineMountain;
	interactiveType = INTER::ROCK;
}

BiomeDes::BiomeDes()
{
	spriteIdx = 1;
	spline = new SplineMountain;
	interactiveType = INTER::NONE;
}

BiomeSnow::BiomeSnow()
{
	spriteIdx = 2;
	spline = new SplineMountain;
	interactiveType = INTER::NONE;
}
