#include "Components.h"

#include "Components/Transform.h"
#include "Components/Renderable.h"

void Components::Start()
{
	AddChild<Transform>()->Start();
	//Node::Start();
}

void Components::Update(double dt)
{
	Node::Update(dt);
}

void Components::End()
{
	Node::End();
}

void Components::LoadPreset(Components::PRESET p)
{
	switch (p)
	{
	default:
	case JSON:
	case EMPTY:
		break;
	case RENDERABLE:
		AddChild<Renderable>()->AttachTransform(GetChild<Transform>())->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("tri"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("placeholder"));
		break;
	}
	Node::Start();
}
