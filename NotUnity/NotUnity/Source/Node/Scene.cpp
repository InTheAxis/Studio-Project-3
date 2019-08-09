#include "Scene.h"
#include "GameObj.h"

#include "../Manager/MgrGraphics.h"
#include "Components/Camera.h"

void Scene::Start()
{		
	MgrGraphics::Instance()->AttachView(AddChild<GameObj>("MainCam")->Create(Components::CAMERA_DEBUG)->GetComp<Camera>()->GetViewMtx());
	AddChild<GameObj>("axes")->Create(Components::AXES);

	AddChild<GameObj>("temp")->Create(Components::RENDERABLE);
	//Node::Start(); //gameobjects start themselves
}

void Scene::Update(double dt)
{
	Node::Update(dt);
}

void Scene::End()
{
	Node::End();
}
