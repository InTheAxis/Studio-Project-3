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
		GetChild<GameObj>("Chunk" + std::to_string(i))->GetTransform()->translate.Set(i * 40, 0, -1);
		GetChild<GameObj>("Chunk" + std::to_string(i))->GetTransform()->scale.Set(40, 22.5, 1);
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
	if (ControllerKeyboard::Instance()->IsKeyPressed('K'))
	{
		for (int i = 0; i < mapSize; ++i)
		{
			GetChild<GameObj>("Chunk" + std::to_string(i))->GetTransform()->scale += Vector3(0.1, 0.1, 0);
		}
		Debug::Log(GetChild<GameObj>("Chunk" + std::to_string(0))->GetTransform()->scale);
	}
	if (ControllerKeyboard::Instance()->IsKeyPressed('L'))
	{
		for (int i = 0; i < mapSize; ++i)
		{
			GetChild<GameObj>("Chunk" + std::to_string(i))->GetTransform()->scale += Vector3(0.1, 0, 0);
		}
		Debug::Log(GetChild<GameObj>("Chunk" + std::to_string(0))->GetTransform()->scale);
	}
	if (ControllerKeyboard::Instance()->IsKeyPressed('I'))
	{
		GetChild<GameObj>("Chunk1")->GetTransform()->translate += Vector3(0.1, 0, 0);
		Debug::Log(GetChild<GameObj>("Chunk1")->GetTransform()->translate);
	}
	if (!camera)
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
					GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate.Set(offsetX + 1, 0, -1);
				}
				else
				{
					--chunkNumber;
					--offsetX;
					GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate.Set(offsetX + 1, 0, -1);
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
				GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate.Set(mapSize + 1, 0, -1);
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