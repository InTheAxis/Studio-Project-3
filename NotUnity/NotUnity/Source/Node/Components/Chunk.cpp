#include "Chunk.h"
#include "../../Utility/WinDebug.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../Components/ColliderRender.h"
#include "../Components/Sprite.h"
#include "../GameObj.h"
#include "../../Utility/Biomes/BiomeBase.h"
#include "../../Utility/Biomes/BiomePicker.h"

Chunk::Chunk(std::string name) : Node(name)
{
}

Chunk::~Chunk()
{
	if (biome)
	{
		delete biome;
		biome = nullptr;
	}
}

void Chunk::OnEnable()
{
}

void Chunk::OnDisable()
{
}

void Chunk::Start()
{
	sprite = AddChild<Sprite>()
		->SetAnimation(0, 1, 0.5f, 0)
		->SetAnimation(1, 1, 0.5f, 0)
		->SetAnimation(2, 1, 0.5f, 0)
		->SwitchAnimation(0)
		->PlayAnimation();
	sprite->SetGameObj(gameObject);
	sprite->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("quad"));
	sprite->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("background"));

	ChangeBiome();
	
	Node::Start();
}

void Chunk::Update(double dt)
{	
	Node::Update(dt);
}

void Chunk::End()
{
	Node::End();
}

Chunk * Chunk::ChangeBiome()
{
	if (biome)
		delete biome;	
	biome = BiomePicker::Pick();
	sprite->SwitchAnimation(biome->GetSpriteIdx())->PlayAnimation();	
	return this;
}

Spline* Chunk::GetSpline()
{
	return biome->GetSpline();
}

Sprite * Chunk::GetSprite()
{
	return sprite;
}
