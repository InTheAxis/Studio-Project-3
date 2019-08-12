#include "MgrMain.h"

#include "MgrScene.h"
#include "MgrGraphics.h"
#include "MgrResource.h"

MgrMain::MgrMain(std::string name) : Manager<MgrMain>(name)
{
}

MgrMain::~MgrMain()
{
}

void MgrMain::Start()
{
	//order matters, make sure not to use generic template

	AddChild("MgrGraphics", MgrGraphics::Instance());
	AddChild("MgrResource", MgrResource::Instance());
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

