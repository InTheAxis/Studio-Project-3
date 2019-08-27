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

BiomeMtFlat::BiomeMtFlat()
{
	spriteIdx = 3;
	spline = new Spline;
	interactiveType = INTER::ROCK;
}

BiomeDesFlat::BiomeDesFlat()
{
	spriteIdx = 4;
	spline = new Spline;
	interactiveType = INTER::NONE;
}

BiomeSnowFlat::BiomeSnowFlat()
{
	spriteIdx = 5;
	spline = new Spline;
	interactiveType = INTER::NONE;
}
