#include "MgrMain.h"

void MgrMain::Start()
{
	Node::Start();
	
	//order matters

	AddChild("MgrScene", MgrScene::Instance());
	AddChild("MgrGraphics", MgrGraphics::Instance());
}

void MgrMain::Update(double dt)
{
	Node::Update(dt);
}

void MgrMain::End()
{
	Node::End();
}
