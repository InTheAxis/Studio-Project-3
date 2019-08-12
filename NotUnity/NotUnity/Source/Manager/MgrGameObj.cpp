#include "MgrGameObj.h"
#include "../Node/Scene.h"
#include "../Node/GameObj.h"

void MgrGameObj::Start()
{
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

void MgrGameObj::RegisterRenderable(GameObj * go)
{
	if (go)
		renderables.emplace_back(go);
}

std::vector<GameObj*>* MgrGameObj::GetRenderables()
{
	return &renderables;
}