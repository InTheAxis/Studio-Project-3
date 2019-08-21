#include "MapGenerator.h"
#include "../Manager/MgrGameObj.h"
#include "../../Node/GameObj.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../../Utility/Math/Splines/Mountain.h"
#include "../GameObj.h"
MapGenerator::MapGenerator(std::string name) :
	Node(name),
	offsetBuffer(0),
	chunkNumber(0),
	offsetX(0),
	scaleX(20),
	cullingAmount(30)
{
}

MapGenerator::~MapGenerator()
{
	if (spline != nullptr)
	{
		delete spline;
		spline = nullptr;
	}
}

void MapGenerator::Start()
{
	offsetBuffer = 1 * scaleX; // Desire ammount * scale
	for (int i = 0; i < mapSize; ++i)
	{
		spline = new SplineMountain;
		spline->SetOffset(i * -scaleX);
		chunkGO[i] = AddChild<GameObj>("Chunk" + std::to_string(i));
		chunkGO[i]->AddComp<Chunk>()->SetSpline(spline);
		chunkGO[i]->GetTransform()->translate.Set(i * scaleX, 0, -1);
		chunkGO[i]->GetTransform()->scale.Set(scaleX, scaleX, 1);
	}
	CullChunk();
	Node::Start();
}

void MapGenerator::Update(double dt)
{
	if (camera)
	{
		if (ControllerKeyboard::Instance()->IsKeyDown(VK_LEFT))
		{
			Vector3 oBoA = camera->GetParent()->GetChild<Transform>()->translate - chunkGO[chunkNumber]->GetTransform()->translate; /*GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate;*/ // take the last chunck pos
			oBoA.y = 0;
			oBoA.z = 0;
			float displacement = oBoA.Length();
			if (displacement < offsetBuffer)
			{
				if (chunkNumber == 0)
				{
					chunkNumber = mapSize - 1;
					--offsetX;
					chunkGO[chunkNumber]->GetTransform()->translate.Set((scaleX * offsetX), 0, -1);
					chunkGO[chunkNumber]->GetComp<Chunk>()->GetSpline()->SetOffset((-scaleX * offsetX));
					CullChunk();
				}
				else
				{
					--chunkNumber;
					--offsetX;
					chunkGO[chunkNumber]->GetTransform()->translate.Set((scaleX * offsetX), 0, -1);
					chunkGO[chunkNumber]->GetComp<Chunk>()->GetSpline()->SetOffset((-scaleX * offsetX));
					CullChunk();
				}
			}
		}
		else if (ControllerKeyboard::Instance()->IsKeyDown(VK_RIGHT))
		{
			Vector3 oBoA = camera->GetParent()->GetChild<Transform>()->translate - chunkGO[chunkNumber]->GetTransform()->translate; // take the last chunck pos
			oBoA.y = 0;
			oBoA.z = 0;
			float displacement = oBoA.Length();
			if (displacement > offsetBuffer)
			{
				chunkGO[chunkNumber]->GetTransform()->translate.Set((mapSize * scaleX) + (offsetX * scaleX), 0, -1);
				chunkGO[chunkNumber]->GetComp<Chunk>()->GetSpline()->SetOffset((-scaleX * offsetX));
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
	Vector3 oBoA;
	for (int i = 0; i < mapSize; ++i)
	{
		oBoA = camera->GetParent()->GetChild<Transform>()->translate - chunkGO[i]->GetTransform()->translate;
		//Debug::Log(camera->GetParent()->GetChild<Transform>()->translate);
		oBoA.y = 0;
		oBoA.z = 0;
		float displacement = oBoA.Length();
		if (displacement <= scaleX * 0.5f)
		{
			return chunkGO[i]->GetComp<Chunk>();
		}
		
	}
}

void MapGenerator::CullChunk()
{
	for (int i = 0; i < mapSize; ++i)
	{
		Vector3 oBoA = camera->GetParent()->GetChild<Transform>()->translate - chunkGO[i]->GetTransform()->translate; // take the last chunck pos
		oBoA.y = 0;
		oBoA.z = 0;
		float displacement = oBoA.Length();
		if (displacement > cullingAmount)
		{
			chunkGO[i]->ActiveSelf(false);
		}
		else
		{
			chunkGO[i]->ActiveSelf(true);
		}
	}
}