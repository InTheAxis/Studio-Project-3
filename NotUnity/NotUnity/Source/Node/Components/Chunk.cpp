#include "Chunk.h"
#include "../../Utility/WinDebug.h"
#include "../../Utility/Math/Spline.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../Components/Sprite.h"
#include "../Components/ColliderRender.h"

Chunk::Chunk(std::string name) : Node(name), HSV(-1,-1,-1) , currAnimIdx(3)
{	
}

Chunk::~Chunk()
{
}

void Chunk::OnEnable()
{
	//if (sprite != nullptr)
	//{
	//	int randomInt = Math::RandIntMinMax(0, 2);
	//	switch (randomInt)
	//	{
	//	case 0:
	//		sprite->SwitchAnimation(randomInt)->PlayAnimation();
	//		break;
	//	case 1:
	//		sprite->SwitchAnimation(randomInt)->PlayAnimation();
	//		break;
	//	case 2:
	//		sprite->SwitchAnimation(randomInt)->PlayAnimation();
	//		break;
	//	default:
	//		sprite->SwitchAnimation(0)->PlayAnimation();
	//		break;
	//	}
	//}
}

void Chunk::OnDisable()
{
	if (sprite)
	{
		currAnimIdx = currAnimIdx + Math::RandIntMinMax(1, 2);
		sprite->SwitchAnimation(Math::Wrap(currAnimIdx, 0, 8))->PlayAnimation();
	}
}

void Chunk::Start()
{
	// background
	sprite = AddChild<Sprite>()
		->SetAnimation(0, 1, 0.5f, 0)
		->SetAnimation(1, 1, 0.5f, 0)
		->SetAnimation(2, 1, 0.5f, 0)
		->SetAnimation(3, 1, 0.5f, 0)
		->SetAnimation(4, 1, 0.5f, 0)
		->SetAnimation(5, 1, 0.5f, 0)
		->SetAnimation(6, 1, 0.5f, 0)
		->SetAnimation(7, 1, 0.5f, 0)
		->SetAnimation(8, 1, 0.5f, 0);
	sprite->SetGameObj(gameObject);
	sprite->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("bg"));
	sprite->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("background"));
	sprite->SelectShader(MgrGraphics::HSV_UNLIT);
	sprite->SetRenderPass(RENDER_PASS::GEO);
	sprite->SwitchAnimation(currAnimIdx)->PlayAnimation();
	//sprite->SetHSV(HSV.x, HSV.y, HSV.z);


	AddChild<GameObj>("Spline")->AddComp<ColliderRender>()->AttachPoints(spline->GetSamplePts())->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("placeholder"))->SetGameObj(gameObject);

	Node::Start();
}

void Chunk::Update(double dt)
{
	Node::Update(dt);
}

void Chunk::End()
{
	if (spline != nullptr)
	{
		delete spline;
		spline = nullptr;
	}
	Node::End();
}

void Chunk::AssignBiome(BIOME_TYPE bt)
{
	switch (bt)
	{
	case BIOME_TYPE::DESERT:
		HSV.Set(-1, -1, -1);
		break;
	case BIOME_TYPE::SNOW:
		HSV.Set(-1, -1, -1);
		break;
	case BIOME_TYPE::GRASS:
		HSV.Set(-1, -1, -1);
		break;
	default:
		Debug::Log("Broke when assigning biome");
		break;
	}
}

void Chunk::SetSpline(Spline * spline)
{
	this->spline = spline;
}

Spline* Chunk::GetSpline()
{
	return spline;
}

Sprite * Chunk::GetSprite()
{
	return sprite;
}
