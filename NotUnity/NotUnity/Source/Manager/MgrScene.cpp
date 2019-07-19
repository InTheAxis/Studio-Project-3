#include "MgrScene.h"

#include "../Node/NScene.h"

void MgrScene::Start()
{
	Node::Start();
	//create scenes here
	AddChild<NScene>("default");
}

void MgrScene::Update(double dt)
{
	Node::Update(dt);
}

void MgrScene::End()
{
	Node::End();
}
