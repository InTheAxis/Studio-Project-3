#include "Scene.h"
#include "Manager/MgrGraphics.h"
#include "Manager/MgrGameObj.h"
#include "Components/Renderable.h"
#include "../Application.h"

Scene::Scene(std::string name) 
	: Node(name)
	, mg(nullptr)
	, mgo(nullptr)	
{
	//get singleton references
	mg = MgrGraphics::Instance();
	mgo = MgrGameObj::Instance();		
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
	RenderPass(RENDER_PASS::FINAL);
}

void Scene::RenderPass(RENDER_PASS rp)
{
	std::vector<Renderable*>* temp = mgo->GetRenderables(rp);
	for (auto r : *temp)
	{
		r->Render();
	}
}