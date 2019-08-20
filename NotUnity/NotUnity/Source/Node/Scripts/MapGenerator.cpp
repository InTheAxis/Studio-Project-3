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
	for (int i = 0; i < mapSize; ++i)
	{
		AddChild<GameObj>("Chunk" + std::to_string(i))->AddComp<Chunk>();
		GetChild<GameObj>("Chunk" + std::to_string(i))->GetTransform()->translate.Set(i * scaleX, 0, -1);
		GetChild<GameObj>("Chunk" + std::to_string(i))->GetTransform()->scale.Set(scaleX, 5.7f, 1);		
		GetChild<GameObj>("Chunk" + std::to_string(i))->GetComp<Chunk>()->AssignBiome(BIOME_TYPE::GRASS);		
	}
	CullChunk();
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
					CullChunk();
				}
				else
				{
					--chunkNumber;
					--offsetX;
					GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate.Set((scaleX * offsetX), 0, -1);
					CullChunk();
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
				CullChunk();
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

void MapGenerator::SetCamera(Camera * camera)
{
	this->camera = camera;
}

Chunk * MapGenerator::GetCurrChunk()
{	
	return GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetComp<Chunk>();
}

void MapGenerator::CullChunk()
{
	for (int i = 0; i < mapSize; ++i)
	{
		Vector3 oBoA = camera->GetParent()->GetChild<Transform>()->translate - GetChild<GameObj>("Chunk" + std::to_string(i))->GetTransform()->translate; // take the last chunck pos
		oBoA.z = 0;
		float displacement = oBoA.Length();
		if (displacement > cullingAmount)
		{
			GetChild<GameObj>("Chunk" + std::to_string(i))->ActiveSelf(false);
		}
		else
		{
			GetChild<GameObj>("Chunk" + std::to_string(i))->ActiveSelf(true);
		}
	}
}
