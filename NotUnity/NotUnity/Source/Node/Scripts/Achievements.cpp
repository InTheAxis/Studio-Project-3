#include "Achievements.h"

Achievements::Achievements(std::string name) : Node(name)
{
}

Achievements::~Achievements()
{
}

void Achievements::Start()
{			
	Node::Start();
}

void Achievements::Update(double dt)
{
	Node::Update(dt);
}

void Achievements::End()
{
	Node::End();
}