#pragma once

#include <sstream>

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Scripts.h"
#include "../Components/Text.h"
#include "../Components/Chunk.h"
#include "../Components/Camera.h"
#include "../Components.h"

class MapGenerator : public Node, public TypeID<MapGenerator>,public Component
{
public:
	MapGenerator(std::string name = "MapGenerator");
	~MapGenerator();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	void setCamera(Camera* camera);
private:
	Camera* camera;
	int mapSize;
	int offsetBuffer; // displacement > offsetBuffer to spawn the new blocks
	int offsetX; // increase overtime when chunk gets pop and push to the back
	int scaleX;
	int chunkNumber;
};