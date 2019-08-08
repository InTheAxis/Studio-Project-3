#include "MgrMain.h"

void MgrMain::Start()
{
	//order matters

	AddChild("MgrScene", MgrScene::Instance());
	AddChild("MgrGraphics", MgrGraphics::Instance());
	
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
