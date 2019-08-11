#include "GameObj.h"

#include "Components/Transform.h"

void GameObj::Start()
{
	comps = AddChild<Components>();
	scripts = comps->AddChild<Scripts>();
	t = comps->AddChild<Transform>();
}

void GameObj::Update(double dt)
{	
	Node::Update(dt);
}

void GameObj::End()
{
	Node::End();
}