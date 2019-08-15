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
	//sprite = gameObject->AddComp<Sprite>();
	//sprite->Start();
	sprite = AddChild<Sprite>();
	sprite->SetGameObj(gameObject);
	sprite->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("quad"));
	sprite->AttachMaterial(material);
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

Chunk * Chunk::SetBiome(BIOME_TYPE bt)
{
	currentBiome = bt;
	return this;
}

void Chunk::assignMaterial(std::string materialName)
{
	material = MgrGraphics::Instance()->GetCachedMaterial(materialName);
}



