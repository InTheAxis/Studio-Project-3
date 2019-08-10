#include "Scene.h"
#include "GameObj.h"

#include "../Manager/MgrGraphics.h"
#include "Components/Camera.h"
#include "Components/Renderable.h"

void Scene::Start()
{		
	MgrGraphics::Instance()->AttachView(AddChild<GameObj>("MainCam")->Create(Components::CAMERA_DEBUG)->GetComp<Camera>()->GetViewMtx());
	
	renderables.emplace_back(AddChild<GameObj>("axes")->Create(Components::AXES)->GetComp<Renderable>());
	renderables.emplace_back(AddChild<GameObj>("temp")->Create(Components::RENDERABLE)->GetComp<Renderable>());
	
	//Node::Start(); //gameobjects start themselves
}

void Scene::Update(double dt)
{
	Node::Update(dt);
	this->Render();
}

void Scene::End()
{
	Node::End();
}

void Scene::Render()
{	
	for (auto r : renderables)
	{
		r->Render();
	}
}