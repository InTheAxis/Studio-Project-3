#include "MgrGameObj.h"
#include "../Scene.h"
#include "../GameObj.h"

MgrGameObj::MgrGameObj(std::string name)  : Manager<MgrGameObj>(name) 
{
}

MgrGameObj::~MgrGameObj()
{
}

void MgrGameObj::Start()
{
	Node::Start();
}

void MgrGameObj::Update(double dt)
{	
	Node::Update(dt);
}

void MgrGameObj::End()
{
	Node::End();
}

GameObj* MgrGameObj::CreateGameObj(std::string name, Scene * ref)
{	
	goList[ref][name] = ref->AddChild<GameObj>(name);
	goList[ref][name]->Start();
	return goList[ref][name];
}

GameObj * MgrGameObj::GetGameObj(std::string name, Scene * ref)
{	
	if (goList[ref].count(name) > 0)
		return goList[ref][name];
	return nullptr;
}

void MgrGameObj::RegisterRenderable(Renderable * go)
{
	if (go)
		renderables.emplace_back(go);
}

std::vector<Renderable*>* MgrGameObj::GetRenderables()
{
	return &renderables;
}
