#include "MapGenerator.h"
#include "../Manager/MgrGameObj.h"
#include "../../Node/GameObj.h"
#include "../../Utility/Input/ControllerKeyboard.h"

MapGenerator::MapGenerator(std::string name) :
	Node(name),
	mapSize(0),
	offsetBuffer(0),
	chunkNumber(0),
	offsetX(0),
	scaleX(10),
	cullingAmount(30)
{
}

MapGenerator::~MapGenerator()
{
}

void MapGenerator::Start()
{
	mapSize = 10;
	offsetBuffer = 1 * scaleX; // Desire ammount * scale
	bool oneTwo = false;
	for (int i = 0; i < mapSize; ++i)
	{
		AddChild<GameObj>("Chunk" + std::to_string(i))->AddComp<Chunk>();
		GetChild<GameObj>("Chunk" + std::to_string(i))->GetTransform()->translate.Set(i * scaleX, 0, -1);
		GetChild<GameObj>("Chunk" + std::to_string(i))->GetTransform()->scale.Set(scaleX, 5.7f, 1);
		GetChild<GameObj>("Chunk" + std::to_string(i))->GetComp<Chunk>()->AssignMaterial("background2");
		GetChild<GameObj>("Chunk" + std::to_string(i))->GetComp<Chunk>()->AssignBiome(BIOME_TYPE::GRASS);
		if (oneTwo)
		{
			oneTwo = false;
			GetChild<GameObj>("Chunk" + std::to_string(i))->GetComp<Chunk>()->AssignMaterial("background");
		}
		else
		{
			oneTwo = true;
		}
	}
	cullChunk();
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
					GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate.Set((scaleX * offsetX), 0, -1);
					cullChunk();
				}
				else
				{
					--chunkNumber;
					--offsetX;
					GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate.Set((scaleX * offsetX), 0, -1);
					cullChunk();
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
				GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate.Set((mapSize * scaleX) + (offsetX * scaleX), 0, -1); // 10 * 10 (to get the end of the map) + (10 * n)
				cullChunk();
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

void MapGenerator::cullChunk()
{
	for (int i = 0; i < mapSize; ++i)
	{
		Vector3 oBoA = camera->GetParent()->GetChild<Transform>()->translate - GetChild<GameObj>("Chunk" + std::to_string(i))->GetTransform()->translate; // take the last chunck pos
		oBoA.z = 0;
		float displacement = oBoA.Length();
		if (displacement > cullingAmount)
		{
			GetChild<GameObj>("Chunk" + std::to_string(i))->ActiveSelf(false);
			Debug::Log("Chunk" + std::to_string(i) + " " + std::to_string(GetChild<GameObj>("Chunk" + std::to_string(i))->IsActive()));
		}
		else
		{
			GetChild<GameObj>("Chunk" + std::to_string(i))->ActiveSelf(true);
		}
	}
}
