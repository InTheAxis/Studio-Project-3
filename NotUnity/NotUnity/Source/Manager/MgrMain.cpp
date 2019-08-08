#include "MgrMain.h"

void MgrMain::Start()
{
	//order matters

	AddChild("MgrGraphics", MgrGraphics::Instance());
	AddChild("MgrScene", MgrScene::Instance());
	
	Node::Start();
}

void MgrMain::Update(double dt)
{
	Node::Update(dt);
}

void MgrMain::End()
{
	Node::End();
}
