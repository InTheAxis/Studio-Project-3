#include "MgrGameObj.h"
#include "MgrScene.h"
#include "../Scene.h"
#include "../GameObj.h"
#include "../Components/Renderable.h"

MgrGameObj::MgrGameObj(std::string name)  : Manager<MgrGameObj>(name)
{
}

MgrGameObj::~MgrGameObj()
{
	goList.clear();
	renderables.clear();
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
		renderables[r->GetRenderPass()].emplace_back(r);
}

void MgrGameObj::UnRegisterRenderable(Renderable * r)
{
	for (auto it = renderables[r->GetRenderPass()].begin(); it != renderables[r->GetRenderPass()].end(); ++it)
	{
		if (*it == r)
		{
			renderables[r->GetRenderPass()].erase(it);
			return;
		}
	}
}

std::vector<Renderable*>* MgrGameObj::GetRenderables(RENDER_PASS renderPass)
{
	return &(renderables[renderPass]);
}
