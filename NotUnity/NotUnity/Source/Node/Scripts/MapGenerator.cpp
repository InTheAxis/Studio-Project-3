#include "MapGenerator.h"
#include "../Manager/MgrGameObj.h"
#include "../../Node/GameObj.h"
#include "../../Utility/Input/ControllerKeyboard.h"

MapGenerator::MapGenerator(std::string name) : 
	Node(name),
	mapSize(0),
	offsetBuffer(0)
{
}

MapGenerator::~MapGenerator()
{
}

void MapGenerator::Start()
{
	mapSize = 10;
	offsetBuffer = 2;
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
		//if (camera->GetParent()->GetChild<Transform>()->translate.x >= 9.0f)
		//{
		//	Debug::Log("Poi");
		//}
		if (floor(camera->GetParent()->GetChild<Transform>()->translate.x) == 9.0f)
		{

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
