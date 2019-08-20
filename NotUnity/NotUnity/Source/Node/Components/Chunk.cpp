#include "Chunk.h"
#include "../../Utility/WinDebug.h"
#include "../../Utility/Math/Spline.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../Components/Sprite.h"
#include "../Components/ColliderRender.h"

Chunk::Chunk(std::string name) : Node(name)
{	
}

Chunk::~Chunk()
{
}

void Chunk::OnEnable()
{
}

void Chunk::OnDisable()
{
}

void Chunk::Start()
{
	// background
	sprite = AddChild<Sprite>();
	sprite->SetGameObj(gameObject);
	sprite->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("quad"));
	sprite->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("background"));
	sprite->SelectShader(MgrGraphics::HSV_UNLIT);
	sprite->SetRenderPass(RENDER_PASS::POST_FX);
	sprite->SetRenderPass(RENDER_PASS::GEO);
	sprite->SetHSV(HSV.x, HSV.y, HSV.z);
	
	spline = new Spline;
	AddChild<GameObj>("Spline")->AddComp<ColliderRender>()->AttachPoints(spline->GetSamplePts())->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("default"))->SetGameObj(gameObject);

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

Spline* Chunk::GetSpline()
{
	return spline;
}
