#include "GameObj.h"

#include "Components.h"
#include "Scripts.h"
#include "Components/Transform.h"

void GameObj::Start()
{
	comps = AddChild<Components>();
	scripts = AddChild<Scripts>();
	Node::Start();
	t = comps->GetChild<Transform>();
	if (!t) Debug::LogError("Cant find transform!");
}

void GameObj::Update(double dt)
{
	//update children manually
	comps->Update(dt);
	scripts->Update(dt);
	//Node::Update(dt);
}

void GameObj::End()
{
	Node::End();
}
