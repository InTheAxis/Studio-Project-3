#include "Components.h"

#include "Components/Transform.h"
#include "Components/Renderable.h"
#include "Components/Sprite.h"
#include "Components/Text.h"
#include "Components/Camera.h"
#include "Components/Axis.h"

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
	case CAMERA_DEBUG:
		AddChild<Camera>()->AttachTransform(GetChild<Transform>())->SetMode(Camera::DEBUG)->SetSpeed(1.5f);
		break;
	case AXES:
		AddChild<Renderable>()->AttachTransform(GetChild<Transform>())->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("axes"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("default"));
		break;
	case SPRITE:
		AddChild<Sprite>()->AttachTransform(GetChild<Transform>())->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("quad"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("placeholder"));
		break;
	case TEXT:
		AddChild<Text>()->AttachTransform(GetChild<Transform>())->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("text"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("font"));
		break;
	}
	Node::Start();
}
