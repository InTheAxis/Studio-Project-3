#include "Scene.h"
#include "Manager/MgrGraphics.h"
#include "Manager/MgrGameObj.h"
#include "Components/Renderable.h"
#include "../Application.h"

Scene::Scene(std::string name) 
	: Node(name)
	, mg(nullptr)
	, mgo(nullptr)
	, renderables(nullptr)
{
	//get singleton references
	mg = MgrGraphics::Instance();
	mgo = MgrGameObj::Instance();
	//get go list references
	renderables = mgo->GetRenderables();
}

Scene::~Scene()
{
}

void Scene::Start()
{	
	Node::Start();
}

void Scene::Update(double dt)
{	
	Node::Update(dt);	
}

void Scene::End()
{
	Node::End();
}

void Scene::Render()
{		
	for (auto r : *renderables)
	{
		r->Render();
	}
}