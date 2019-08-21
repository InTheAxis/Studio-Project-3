#ifndef CHUNK_H
#define CHUNK_H

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../../Utility/Math/Vector3.h"
#include "../../Utility/Math/Spline.h"

#include <vector>

enum class BIOME_TYPE
{
	DESERT,
	SNOW,
	GRASS,
	TOTAL_BIOME
};

class Sprite;
class Spline;
class Chunk : public Node, public TypeID<Chunk>, public Component
{
	
public:
	Chunk(std::string name = "Chunk");
	~Chunk();

	void OnEnable();

	void OnDisable();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	void AssignBiome(BIOME_TYPE bt);
	
	void SetSpline(Spline* spline);
	Spline* GetSpline();
private:
	Sprite* sprite;
	Spline* spline;

	Transform* t;
	Vector3 HSV;

	BIOME_TYPE currentBiome;
};

#endif