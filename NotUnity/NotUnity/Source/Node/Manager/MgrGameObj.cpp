#include "MgrGameObj.h"
#include "MgrScene.h"
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

GameObj * MgrGameObj::RegisterGO(std::string name, GameObj * go)
{
	goList[MgrScene::Instance()->GetRoot()][name] = go;
	return go;
}

GameObj * MgrGameObj::FindGO(std::string name)
{
	if (goList[MgrScene::Instance()->GetRoot()].count(name) > 0)
		return goList[MgrScene::Instance()->GetRoot()][name];
	return nullptr;	
}

void MgrGameObj::RegisterRenderable(Renderable * r)
{
	if (r)
		renderables.emplace_back(r);
}

std::vector<Renderable*>* MgrGameObj::GetRenderables()
{
	return &renderables;
}
