#ifndef CHUNK_H
#define CHUNK_H

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../../Utility/Math/Vector3.h"
#include "../../Utility/Math/Spline.h"

#include <vector>

class GameObj;
class Sprite;
class Biome;
enum class INTER;
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

	Chunk* ChangeBiome();

	Spline* GetSpline();
	Sprite* GetSprite();	
private:	
	//refs
	Sprite* sprite;

	//biome presets;
	Biome* biome;
};

#endif