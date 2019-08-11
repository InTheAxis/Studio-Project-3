#include "MgrScene.h"

#include "../Node/Scene.h"

void MgrScene::Start()
{
	//create scenes here, and set inactive accordingly
	AddChild<Scene>("default")->ActiveSelf(true);
}

void MgrScene::Update(double dt)
{	
	Node::Update(dt);
}

void MgrScene::End()
{
	Node::End();
}
