#include "MgrScene.h"

#include "../Scene.h"

MgrScene::MgrScene(std::string name) : Manager<MgrScene>(name)
{
}

MgrScene::~MgrScene()
{
}

void MgrScene::Start()
{
	//create scenes here, and set inactive accordingly
	AddChild<Scene>("default")->ActiveSelf(true);
	Node::Start();
}

void MgrScene::Update(double dt)
{	
	Node::Update(dt);
}

void MgrScene::End()
{
	Node::End();
}

