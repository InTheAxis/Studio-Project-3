#include "MapGenerator.h"
#include "../../Application.h"
#include "../GameObj.h"
#include "../../Node/GameObj.h"
#include "../Manager/MgrGameObj.h"
#include "../Components/Sprite.h"
#include "../../Utility/Input/ControllerKeyboard.h"

MapGenerator::MapGenerator(std::string name) :
	Node(name),
	offsetBuffer(0),
	chunkLeft(0),
	chunkRight(mapSize - 1),
	offsetX(0),
	scaleX(15)	
{
}

MapGenerator::~MapGenerator()
{
}

void MapGenerator::Start()
{
	offsetBuffer = 1.5f * scaleX; // Desire ammount * scale
	for (int i = 0; i < mapSize; ++i)
	{
		chunkGO[i] = AddChild<GameObj>("Chunk" + std::to_string(i));
		chunkGO[i]->AddComp<Chunk>();
		chunkGO[i]->GetTransform()->translate.Set(i * scaleX, 0, -1);
		chunkGO[i]->GetTransform()->scale.Set(scaleX, scaleX, 1);
	}
	CullChunk();

	sky = AddChild<GameObj>("Sky");
	sky->AddComp<Sprite>()
		->SetHSV(-1, 1, -1)->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("sky"))
		->SetRenderPass(RENDER_PASS::GEO)->SelectShader(MgrGraphics::HSV_UNLIT)
		->SetToScroll(Vector2(0.1f, 0));
	sky->GetTransform()->translate = camera->GetParent()->GetChild<Transform>()->translate;
	sky->GetTransform()->scale.Set(15, 15, 1);	
	
	Node::Start();

	for (int i = 0; i < mapSize; ++i)
		chunkGO[i]->GetComp<Chunk>()->GetSpline()->SetOffset((-i * scaleX));
}

void MapGenerator::Update(double dt)
{	
	if (camera)
	{
		sky->GetTransform()->translate.x = camera->GetParent()->GetChild<Transform>()->translate.x;		
		sky->GetTransform()->translate.z = -10;		
		sky->GetTransform()->translate.y = 2;		

		if (GetDisplacement(chunkLeft) > offsetBuffer + scaleX)
		{
			MoveChunk(chunkLeft, (mapSize + offsetX) * scaleX);
			UpdateChunkNum(1);				
		}			
		if (GetDisplacement(chunkRight) > offsetBuffer)
		{
			MoveChunk(chunkRight, (offsetX - 1) * scaleX);
			UpdateChunkNum(-1);			
		}
		//Debug::Log(std::to_string(chunkLeft) + " " + std::to_string(chunkRight));
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
	for (int i = 0; i < mapSize; ++i)
		if (GetDisplacement(i) <= scaleX * 0.5f)
			return chunkGO[i]->GetComp<Chunk>();
}

GameObj * MapGenerator::GetChunkGO(int idx)
{
	if (idx < mapSize && idx >= 0)
		return chunkGO[idx];
	return nullptr;
}

GameObj * MapGenerator::GetSky()
{
	return sky;
}

void MapGenerator::UpdateChunkNum(int incrementAmt)
{
	chunkLeft = Math::Wrap(chunkLeft + incrementAmt, 0, mapSize - 1);
	chunkRight = Math::Wrap(chunkRight + incrementAmt, 0, mapSize - 1);	
	offsetX += incrementAmt;	
}

void MapGenerator::MoveChunk(int chunkIdx, float xPos)
{	
	chunkGO[chunkIdx]->GetTransform()->translate.Set(xPos, 0, -1);
	chunkGO[chunkIdx]->GetComp<Chunk>()->ChangeBiome()->GetSpline()->SetOffset((-xPos));
	CullChunk();	
}

float MapGenerator::GetDisplacement(int chunkIdx)
{
	return Math::FAbs(chunkGO[chunkIdx]->GetTransform()->translate.x - camera->GetParent()->GetChild<Transform>()->translate.x);
}

void MapGenerator::CullChunk()
{
	for (int i = 0; i < mapSize; ++i)
		chunkGO[i]->ActiveSelf(!(GetDisplacement(i) > offsetBuffer));
}


//#include "MapGenerator.h"
//#include "../Manager/MgrGameObj.h"
//#include "../../Node/GameObj.h"
//#include "../../Utility/Input/ControllerKeyboard.h"
//#include "../../Utility/Math/Splines/Mountain.h"
//#include "../GameObj.h"
//#include "../Components/Sprite.h"
//#include "../../Application.h"
//
//MapGenerator::MapGenerator(std::string name) :
//	Node(name),
//	offsetBuffer(0),
//	chunkNumber(0),
//	offsetX(0),
//	scaleX(15),
//	cullingAmount(30)
//{
//}
//
//MapGenerator::~MapGenerator()
//{
//	if (spline != nullptr)
//	{
//		delete spline;
//		spline = nullptr;
//	}
//}
//
//void MapGenerator::Start()
//{
//	offsetBuffer = 1 * scaleX; // Desire ammount * scale
//	for (int i = 0; i < mapSize; ++i)
//	{
//		//spline = new SplineMountain;
//		//spline->SetOffset(i * -scaleX);
//		// chunkGO[i]->AddComp<Chunk>()->SetSpline(spline);	
//		chunkGO[i] = AddChild<GameObj>("Chunk" + std::to_string(i));
//		chunkGO[i]->AddComp<Chunk>()->PopulateSplineList()->SetSplineOffset(i * -scaleX);
//		chunkGO[i]->GetTransform()->translate.Set(i * scaleX, 0, -5);
//		chunkGO[i]->GetTransform()->scale.Set(scaleX, scaleX, 1);
//	}
//	CullChunk();
//
//	sky = AddChild<GameObj>("Sky");
//	sky->AddComp<Sprite>()
//		->SetHSV(-1, 1, -1)->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("sky"))
//		->SetRenderPass(RENDER_PASS::GEO)->SelectShader(MgrGraphics::HSV_UNLIT)
//		->SetToScroll(Vector2(0.1f, 0));
//	sky->GetTransform()->translate = camera->GetParent()->GetChild<Transform>()->translate;
//	sky->GetTransform()->scale.Set(15, 15, 1);
//	Node::Start();
//}
//
//void MapGenerator::Update(double dt)
//{
//	if (camera)
//	{
//		sky->GetTransform()->translate = camera->GetParent()->GetChild<Transform>()->translate;
//		sky->GetTransform()->translate.z = -10;
//		sky->GetTransform()->translate.y = 2;
//
//
//		if (ControllerKeyboard::Instance()->IsKeyDown(VK_LEFT) || ControllerKeyboard::Instance()->IsKeyDown('A'))
//		{
//			Vector3 oBoA = camera->GetParent()->GetChild<Transform>()->translate - chunkGO[chunkNumber]->GetTransform()->translate; /*GetChild<GameObj>("Chunk" + std::to_string(chunkNumber))->GetTransform()->translate;*/ // take the last chunck pos
//			oBoA.y = 0;
//			oBoA.z = 0;
//			float displacement = oBoA.Length();
//			if (displacement < offsetBuffer)
//			{
//				if (chunkNumber == 0)
//				{
//					chunkNumber = mapSize - 1;
//					--offsetX;
//					chunkGO[chunkNumber]->GetTransform()->translate.Set((scaleX * offsetX), 0, -1);
//					chunkGO[chunkNumber]->GetComp<Chunk>()->GetSpline()->SetOffset((-scaleX * offsetX));
//					CullChunk();
//				}
//				else
//				{
//					--chunkNumber;
//					--offsetX;
//					chunkGO[chunkNumber]->GetTransform()->translate.Set((scaleX * offsetX), 0, -1);
//					chunkGO[chunkNumber]->GetComp<Chunk>()->GetSpline()->SetOffset((-scaleX * offsetX));
//					CullChunk();
//				}
//			}
//		}
//		else if (ControllerKeyboard::Instance()->IsKeyDown(VK_RIGHT) || ControllerKeyboard::Instance()->IsKeyDown('D'))
//		{
//			Vector3 oBoA = camera->GetParent()->GetChild<Transform>()->translate - chunkGO[chunkNumber]->GetTransform()->translate; // take the last chunck pos
//			oBoA.y = 0;
//			oBoA.z = 0;
//			float displacement = oBoA.Length();
//			if (displacement > offsetBuffer)
//			{
//				chunkGO[chunkNumber]->GetTransform()->translate.Set((mapSize * scaleX) + (offsetX * scaleX), 0, -1);
//				chunkGO[chunkNumber]->GetComp<Chunk>()->GetSpline()->SetOffset((-scaleX * offsetX));
//				CullChunk();
//				++chunkNumber;
//				++offsetX;
//			}
//			// once reach the end of the chunk, get back the first chunk to recaculate
//			if (chunkNumber == mapSize)
//				chunkNumber = 0;
//		}
//	}
//
//	Node::Update(dt);
//}
//
//void MapGenerator::End()
//{
//	Node::End();
//}
//
//void MapGenerator::SetCamera(Camera * camera)
//{
//	this->camera = camera;
//}
//
//Chunk * MapGenerator::GetCurrChunk()
//{
//	Vector3 oBoA;
//	for (int i = 0; i < mapSize; ++i)
//	{
//		oBoA = camera->GetParent()->GetChild<Transform>()->translate - chunkGO[i]->GetTransform()->translate;
//		oBoA.y = 0;
//		oBoA.z = 0;
//		float displacement = oBoA.Length();
//		if (displacement <= scaleX * 0.5f)
//		{
//			return chunkGO[i]->GetComp<Chunk>();
//		}
//	}
//}
//
//GameObj * MapGenerator::GetChunkGO(int idx)
//{
//	if (idx < mapSize && idx >= 0)
//		return chunkGO[idx];
//	return nullptr;
//}
//
//GameObj * MapGenerator::GetSky()
//{
//	return sky;
//}
//
//void MapGenerator::CullChunk()
//{
//	for (int i = 0; i < mapSize; ++i)
//	{
//		Vector3 oBoA = camera->GetParent()->GetChild<Transform>()->translate - chunkGO[i]->GetTransform()->translate; // take the last chunck pos
//		oBoA.y = 0;
//		oBoA.z = 0;
//		float displacement = oBoA.Length();
//		if (displacement > cullingAmount)
//		{
//			chunkGO[i]->ActiveSelf(false);
//		}
//		else
//		{
//			chunkGO[i]->ActiveSelf(true);
//		}
//	}
//}