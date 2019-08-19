#include "Chunk.h"
#include "../../Utility/WinDebug.h"
#include "../../Utility/Input/ControllerKeyboard.h"

Chunk::Chunk(std::string name) : Node(name)
{
	material = MgrGraphics::Instance()->GetCachedMaterial("background");
}

Chunk::~Chunk()
{
}

void Chunk::Start()
{
	// background
	sprite = AddChild<Sprite>();
	sprite->SetGameObj(gameObject);
	sprite->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("quad"));
	sprite->AttachMaterial(material);
	sprite->SelectShader(MgrGraphics::HSV_UNLIT);
	sprite->SetRenderPass(RENDER_PASS::POST_FX);
	sprite->SetHSV(HSV.x, HSV.y, HSV.z);
	sprite->SetRenderPass(RENDER_PASS::GEO);

	// Spline
	SplineCubic s1;
	SplineExpo s2;
	SplineLogarithm s3;
	SplineQuadratic s4;
	SplineReciprocal s5;
	SplineSqReci s6;

	AddChild<GameObj>("SpCubic")->AddComp<ColliderRender>()->AttachPoints(s1.GetSamplePts())->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("default"));
	AddChild<GameObj>("SpExpo")->AddComp<ColliderRender>()->AttachPoints(s2.GetSamplePts())->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("default"));
	AddChild<GameObj>("SpLoga")->AddComp<ColliderRender>()->AttachPoints(s3.GetSamplePts())->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("default"));
	AddChild<GameObj>("SpQuad")->AddComp<ColliderRender>()->AttachPoints(s4.GetSamplePts())->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("default"));
	AddChild<GameObj>("SpReci")->AddComp<ColliderRender>()->AttachPoints(s5.GetSamplePts())->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("default"));
	AddChild<GameObj>("SpSqReci")->AddComp<ColliderRender>()->AttachPoints(s6.GetSamplePts())->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("default"));
	t = parent->GetChild<Transform>();
	// Debug::Log(t->translate);
	GetChild<GameObj>("SpCubic")->GetTransform()->translate = t->translate;
	GetChild<GameObj>("SpExpo")->GetTransform()->translate = t->translate;
	GetChild<GameObj>("SpLoga")->GetTransform()->translate = t->translate;
	GetChild<GameObj>("SpQuad")->GetTransform()->translate = t->translate;
	GetChild<GameObj>("SpReci")->GetTransform()->translate = t->translate;
	GetChild<GameObj>("SpSqReci")->GetTransform()->translate = t->translate;

	Node::Start();
}

void Chunk::Update(double dt)
{
	GetChild<GameObj>("SpCubic")->GetTransform()->translate = t->translate;
	GetChild<GameObj>("SpExpo")->GetTransform()->translate = t->translate;
	GetChild<GameObj>("SpLoga")->GetTransform()->translate = t->translate;
	GetChild<GameObj>("SpQuad")->GetTransform()->translate = t->translate;
	GetChild<GameObj>("SpReci")->GetTransform()->translate = t->translate;
	GetChild<GameObj>("SpSqReci")->GetTransform()->translate = t->translate;
	Node::Update(dt);
}

void Chunk::End()
{
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

void Chunk::AssignMaterial(std::string materialName)
{
	material = MgrGraphics::Instance()->GetCachedMaterial(materialName);
}