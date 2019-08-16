#include "MapGenerator.h"
#include "../Manager/MgrGameObj.h"
#include "../../Node/GameObj.h"
#include "../../Utility/Input/ControllerKeyboard.h"

MapGenerator::MapGenerator(std::string name) :
	Node(name),
	mapSize(0),
	offsetBuffer(0),
	chunkNumber(0),
	offsetX(0)
{
}

MapGenerator::~MapGenerator()
{
}

void MapGenerator::Start()
{
	mapSize = 10;
	offsetBuffer = 7;
	bool oneTwo = false;
	for (int i = 0; i < mapSize; ++i)
	{
		AddChild<GameObj>("Chunk" + std::to_string(i))->AddComp<Chunk>();
		GetChild<GameObj>("Chunk" + std::to_string(i))->GetTransform()->translate.Set(i, 0, 0);
		GetChild<GameObj>("Chunk" + std::to_string(i))->GetComp<Chunk>()->assignMaterial("background");
		if (oneTwo)
		{
			oneTwo = false;
			GetChild<GameObj>("Chunk" + std::to_string(i))->GetComp<Chunk>()->assignMaterial("background2");
		}
		else
		{
			oneTwo = true;
		}
	}
	Node::Start();
}

void MapGenerator::Update(double dt)
{
	if (camera)
	{
		if (ControllerKeyboard::Instance()->IsKeyDown('A'))
		{
			Vector3 oBoA = camera->GetParent()->GetChild<Transform>()->translate - GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate; // take the last chunck pos
			oBoA.z = 0;
			float displacement = oBoA.Length();
			if (displacement < offsetBuffer)
			{
				if (chunkNumber == 0)
				{
					chunkNumber = mapSize - 1;
					--offsetX;
					GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate.Set(offsetX, 0, 0);
				}
				else
				{
					--chunkNumber;
					--offsetX;
					GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate.Set(offsetX, 0, 0);
				}
			}
		}
		else if (ControllerKeyboard::Instance()->IsKeyDown('D'))
		{
			Vector3 oBoA = camera->GetParent()->GetChild<Transform>()->translate - GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate; // take the last chunck pos
			oBoA.z = 0;
			float displacement = oBoA.Length();
			if (displacement > offsetBuffer)
			{
				GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate.Set(mapSize + offsetX, 0, 0);
				++chunkNumber;
				++offsetX;
			}
			// once reach the end of the chunk, get back the first chunk to recaculate
			if (chunkNumber == mapSize)
				chunkNumber = 0;
		}
	}
	Node::Update(dt);
}

void MapGenerator::End()
{
	Node::End();
}

void MapGenerator::setCamera(Camera * camera)
{
	this->camera = camera;
}