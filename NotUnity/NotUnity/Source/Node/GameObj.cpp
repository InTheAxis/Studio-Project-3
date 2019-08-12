#include "GameObj.h"

#include "Components/Transform.h"

GameObj::GameObj(std::string name) : Node(name)
{
	comps = AddChild<Components>();
	scripts = comps->AddChild<Scripts>();
	t = comps->AddChild<Transform>();
}

GameObj::~GameObj()
{
}

void GameObj::Start()
{
	Node::Start();
}

void GameObj::Update(double dt)
{	
	Node::Update(dt);
}

void GameObj::End()
{
	Node::End();
}