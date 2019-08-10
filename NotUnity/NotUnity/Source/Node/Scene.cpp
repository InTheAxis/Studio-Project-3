#include "Scene.h"
#include "GameObj.h"

#include <sstream>

#include "../Manager/MgrGraphics.h"
#include "Components/Camera.h"
#include "Components/Renderable.h"
#include "Components/Text.h"

void Scene::Start()
{		
	MgrGraphics::Instance()->AttachView(AddChild<GameObj>("MainCam")->Create(Components::CAMERA_DEBUG)->GetComp<Camera>()->GetViewMtx());
	
	renderables.emplace_back(AddChild<GameObj>("axes")->Create(Components::AXES)->GetComp<Renderable>());
	renderables.emplace_back(AddChild<GameObj>("text")->Create(Components::TEXT)->GetComp<Renderable>());
	
	//Node::Start(); //gameobjects start themselves
}

void Scene::Update(double dt)
{
	std::stringstream ss; 
	ss.precision(3);
	ss << "FPS: " << 1.0 / dt;

	GetChild<GameObj>("text")->GetComp<Text>()->SetText(ss.str())->SetColor(Vector4(1,0,0,1));
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