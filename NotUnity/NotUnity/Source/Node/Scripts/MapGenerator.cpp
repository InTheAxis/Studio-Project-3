#include "MapGenerator.h"
#include "../Manager/MgrGameObj.h"
#include "../../Node/GameObj.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../../Utility/Math/Splines/Mountain.h"
#include "../GameObj.h"
#include "../Components/Sprite.h"
#include "../../Application.h"

MapGenerator::MapGenerator(std::string name) :
	Node(name),
	offsetBuffer(0),
	chunkNumber(0),
	offsetX(0),
	scaleX(15)	
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
	offsetBuffer = 1.5f * scaleX; // Desire ammount * scale
	for (int i = 0; i < mapSize; ++i)
	{
		chunkGO[i] = AddChild<GameObj>("Chunk" + std::to_string(i));
		chunkGO[i]->AddComp<Chunk>()->PopulateSplineList()->SetSplineOffset(i * -scaleX);
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
}

void MapGenerator::Update(double dt)
{
	if (camera)
	{
		sky->GetTransform()->translate.x = camera->GetParent()->GetChild<Transform>()->translate.x;		
		sky->GetTransform()->translate.z = -10;		
		sky->GetTransform()->translate.y = 2;		

		float displacement = GetDisplacement(chunkNumber);
		if (displacement < offsetBuffer)
		{
			UpdateChunkNum(-1);			
			MoveChunk(chunkNumber, scaleX * offsetX);
		}
		else if (displacement > offsetBuffer)
		{
			MoveChunk(chunkNumber, (mapSize + offsetX) * scaleX);
			UpdateChunkNum(1);			
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
	chunkNumber = Math::Wrap(chunkNumber + incrementAmt, 0, mapSize - 1);
	offsetX += incrementAmt;
}

void MapGenerator::MoveChunk(int chunkIdx, float xPos)
{	
	chunkGO[chunkIdx]->GetTransform()->translate.Set(xPos, 0, -1);
	chunkGO[chunkIdx]->GetComp<Chunk>()->GetSpline()->SetOffset((-scaleX * offsetX));
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