#include "Chunk.h"
#include "../../Utility/WinDebug.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../Components/Sprite.h"
#include "../../Utility/Math/Spline.h"
#include "../../Utility/Math/Splines/Mountain.h"
#include "../Components/ColliderRender.h"
#include "../GameObj.h"
Chunk::Chunk(std::string name) : Node(name), HSV(-1, -1, -1), currAnimIdx(3)
{
}

Chunk::~Chunk()
{
	// DELETE GO LIST, SPLINE LIST, SPLINE, SPRITE
}

void Chunk::OnEnable()
{
}

void Chunk::OnDisable()
{
	int randInt = 1; //Math::RandIntMinMax(0,1);
	switch (randInt)
	{
	case (int)SPLINE_TYPE::XYLINE:
		if (sprite)
		{
			UpdateMap(SPLINE_TYPE::XYLINE);
			//currAnimIdx = currAnimIdx + Math::RandIntMinMax(1, 2); // switch to different biome?
			//sprite->SwitchAnimation(Math::Wrap(currAnimIdx, 0, 8))->PlayAnimation(); // switch to different biome?
		}
		break;
	case (int)SPLINE_TYPE::MOUNTAIN:
		if (sprite)
		{
			UpdateMap(SPLINE_TYPE::MOUNTAIN);
			//AssignBiome(Math::Wrap(currAnimIdx, 0, 8));
			// currAnimIdx = currAnimIdx + Math::RandIntMinMax(1, 2);  // switch to different biome?
			// sprite->SwitchAnimation(Math::Wrap(currAnimIdx, 0, 8))->PlayAnimation();  // switch to different biome?
		}
		break;
	default:
		break;
	}
}

void Chunk::Start()
{
	int randInt = 1;//Math::RandIntMinMax(0, 1);
	switch (randInt)
	{
	case (int)SPLINE_TYPE::XYLINE:
		GenerateMap(SPLINE_TYPE::XYLINE);
		break;
	case (int)SPLINE_TYPE::MOUNTAIN:
		GenerateMap(SPLINE_TYPE::MOUNTAIN);
		break;
	default:
		break;
	}
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

void Chunk::AssignBiome()
{
	switch (currentBiome)
	{
	case BIOME_TYPE::DESERT:
		int temp = Math::RandIntMinMax(1, 2);
		switch (temp)
		{
		case 1:
			currentBiome = BIOME_TYPE::DESERT;
			currAnimIdx = 6;
			sprite->SwitchAnimation(currAnimIdx)->PlayAnimation();
			break;
		case 2:
			currentBiome = BIOME_TYPE::GRASS;
			currAnimIdx = 3;
			sprite->SwitchAnimation(currAnimIdx)->PlayAnimation();
			break;
		default:
			Debug::Log("Switch case went kapot");
			break;
		}
		break;
	case BIOME_TYPE::SNOW:
		int temp = Math::RandIntMinMax(1, 2);
		switch (temp)
		{
		case 1:
			currentBiome = BIOME_TYPE::SNOW;
			currAnimIdx = 0;
			sprite->SwitchAnimation(currAnimIdx)->PlayAnimation();
			break;
		case 2:
			currentBiome = BIOME_TYPE::GRASS;
			currAnimIdx = 0;
			sprite->SwitchAnimation(currAnimIdx)->PlayAnimation();
			break;
		default:
			break;
		}
		break;
	case BIOME_TYPE::GRASS:
		int temp = Math::RandIntMinMax(1, 3);
		switch (temp)
		{
		case 1:
			currentBiome = BIOME_TYPE::SNOW;
			currAnimIdx = 0;
			sprite->SwitchAnimation(currAnimIdx)->PlayAnimation();
			break;
		case 2:
			currentBiome = BIOME_TYPE::GRASS;
			currAnimIdx = 3;
			sprite->SwitchAnimation(currAnimIdx)->PlayAnimation();
			break;
		case 3:
			currentBiome = BIOME_TYPE::DESERT;
			currAnimIdx = 6;
			sprite->SwitchAnimation(currAnimIdx)->PlayAnimation();
			break;
		default:
			Debug::Log("switch case went kapot");
			break;
		}
		break;
	default:
		break;
	}
}

void Chunk::AssignBiome(int i)
{
	if (i >= 0 && i <= 2)
	{
		currentBiome = BIOME_TYPE::SNOW;
	}
	else if (i >= 3 && i <= 5)
	{
		currentBiome = BIOME_TYPE::GRASS;
	}
	else if (i >= 6 && i <= 8)
	{
		currentBiome = BIOME_TYPE::DESERT;
	}
}

void Chunk::SetSpline(Spline * spline)
{
	this->spline = spline;
}

Chunk* Chunk::SetSplineOffset(float offset)
{
	for (auto go : SplineList)
	{
		go->SetOffset(offset);
	}
	return this;
}

Spline* Chunk::GetSpline()
{
	return spline;
}

Sprite * Chunk::GetSprite()
{
	return sprite;
}

void Chunk::GenerateMap(SPLINE_TYPE ST)
{
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
	switch (ST)
	{
	case SPLINE_TYPE::XYLINE:
		// Depending on what is
		//temp->GetComp<Sprite>("map")->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("background2"));

		sprite->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("background2"));
		//spline = SplineList[(int)SPLINE_TYPE::XYLINE];
		break;
	case SPLINE_TYPE::MOUNTAIN:
		//temp->GetComp<Sprite>("map")->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("background"));
		//Debug::Log((int)SPLINE_TYPE::MOUNTAIN);
		sprite->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("background"));
		spline = SplineList[(int)SPLINE_TYPE::MOUNTAIN];
		break;
	default:
		Debug::Log("Spoil, wrong Spline type");
		break;
	}
	sprite->SelectShader(MgrGraphics::HSV_UNLIT);
	sprite->SetRenderPass(RENDER_PASS::GEO);
	sprite->SwitchAnimation(currAnimIdx)->PlayAnimation();
	currentBiome = BIOME_TYPE::GRASS;
}

void Chunk::UpdateMap(SPLINE_TYPE ST)
{
	switch (ST)
	{
	case SPLINE_TYPE::XYLINE:
		sprite->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("background2"));
		AssignBiome();
		// spline = SplineList[(int)SPLINE_TYPE::XYLINE];
		break;
	case SPLINE_TYPE::MOUNTAIN:
		sprite->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("background"));
		spline = SplineList[(int)SPLINE_TYPE::MOUNTAIN];
		AssignBiome();
		break;
	default:
		Debug::Log("Spoil, wrong Spline type");
		break;
	}
}

Chunk* Chunk::PopulateSplineList() // Add in new splines when needed
{
	spline = new Spline;
	SplineList.push_back(spline);

	spline = new SplineMountain;
	SplineList.push_back(spline);

	return this;
}