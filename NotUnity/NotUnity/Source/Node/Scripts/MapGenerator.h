#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include <sstream>

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Scripts.h"
#include "../Components/Text.h"
#include "../Components/Chunk.h"
#include "../Components/Camera.h"
#include "../Components.h"

class GameObj;
class MapGenerator : public Node, public TypeID<MapGenerator>,public Component
{
public:
	MapGenerator(std::string name = "MapGenerator");
	~MapGenerator();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	void SetCamera(Camera* camera);
	Chunk* GetCurrChunk();

	const static int mapSize = 10;
	GameObj* GetChunkGO(int idx);
	GameObj* GetSky();
private:
	GameObj* chunkGO[mapSize];
	GameObj* sky;
	Camera* camera;

	int offsetBuffer; //displacement > offsetBuffer to spawn the new blocks
	int offsetX; // increase over time when chunk gets pop and push to the back
	int scaleX; //scale of the chunk		
	int chunkLeft, chunkRight; //leftmost and rightmost chunk

	void UpdateChunkNum(int incrementAmt);
	void MoveChunk(int chunkIdx, float xPos);
	float GetDisplacement(int chunkIdx);
	void CullChunk();
};

#endif