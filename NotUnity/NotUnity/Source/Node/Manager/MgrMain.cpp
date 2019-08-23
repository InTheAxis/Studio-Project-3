#include "MgrMain.h"

#include "MgrScene.h"
#include "MgrGraphics.h"
#include "MgrResource.h"
#include "MgrCollision.h"
#include "MgrSound.h"
#include "MgrSkillTree.h"
#include "MgrAchievements.h"

MgrMain::MgrMain(std::string name) : Manager<MgrMain>(name)
{
}

MgrMain::~MgrMain()
{
}

void MgrMain::Start()
{
	//order matters, make sure not to use generic template
	allManagers.emplace_back(MgrGraphics::Instance());
	allManagers.emplace_back(MgrSound::Instance());
	allManagers.emplace_back(MgrResource::Instance());
	allManagers.emplace_back(MgrScene::Instance());
	allManagers.emplace_back(MgrCollision::Instance());
	allManagers.emplace_back(MgrSkillTree::Instance());
	allManagers.emplace_back(MgrAchievements::Instance());

	for (auto m : allManagers)
		m->Start();

	Node::Start();
}

void MgrMain::Update(double dt)
{
	for (auto m : allManagers)
		m->Update(dt);
	Node::Update(dt);
}

void MgrMain::End()
{
	for (auto m : allManagers)
		m->End();
	Node::End();	
}

