#ifndef CHUNK_H
#define CHUNK_H

#include "../Node.h"
#include "../Components/ColliderRender.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../../Utility/Math/Vector3.h"
\
#include "../../Utility/Math/Splines/Cubic.h"
#include "../../Utility/Math/Splines/Exponential.h"
#include "../../Utility/Math/Splines/Logarithm.h"
#include "../../Utility/Math/Splines/Quadratic.h"
#include "../../Utility/Math/Splines/Reciprocal.h"
#include "../../Utility/Math/Splines/SqReci.h"

#include <vector>
#include "Sprite.h"

class Chunk : public Node, public TypeID<Chunk>, public Component
{
	enum BIOME_TYPE
	{
		DESERT,
		SNOW,
		GRASS,
		TOTAL_BIOME
	};
public:
	Chunk(std::string name = "Chunk");
	~Chunk();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	Chunk* SetBiome(BIOME_TYPE bt);
	void assignMaterial(std::string materialName);
private:
	Sprite* sprite;
	Material* material;
	Transform* t;

	BIOME_TYPE currentBiome;
};

#endif