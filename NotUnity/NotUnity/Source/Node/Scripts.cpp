#include "Scripts.h"

Scripts::Scripts(std::string name) : Node(name)
{
}

Scripts::~Scripts()
{
}

void Scripts::Start()
{
	Node::Start();
}

void Scripts::Update(double dt)
{
	Node::Update(dt);
}

void Scripts::End()
{
	Node::End();
}