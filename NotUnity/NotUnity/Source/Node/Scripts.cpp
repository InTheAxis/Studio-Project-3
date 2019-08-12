#include "Scripts.h"

ScriptContainer::ScriptContainer(std::string name) : Node(name)
{
}

ScriptContainer::~ScriptContainer()
{
}

void ScriptContainer::Start()
{
	Node::Start();
}

void ScriptContainer::Update(double dt)
{
	Node::Update(dt);
}

void ScriptContainer::End()
{
	Node::End();
}