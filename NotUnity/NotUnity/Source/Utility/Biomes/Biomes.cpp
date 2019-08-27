#include "Biomes.h"

#include "../Math/Splines/Mountain.h"
#include "../../Node/GameObj.h"

BiomeMt::BiomeMt()
{
	spriteIdx = 0;
	spline = new SplineMountain;
	interactiveType = INTER::CRATE;
}

BiomeDes::BiomeDes()
{
	spriteIdx = 1;
	spline = new SplineMountain;
	interactiveType = INTER::CRATE;
}

BiomeSnow::BiomeSnow()
{
	spriteIdx = 2;
	spline = new SplineMountain;
	interactiveType = INTER::CRATE;
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
	interactiveType = INTER::ROCK;
}

BiomeSnowFlat::BiomeSnowFlat()
{
	spriteIdx = 5;
	spline = new Spline;
	interactiveType = INTER::ROCK;
}
