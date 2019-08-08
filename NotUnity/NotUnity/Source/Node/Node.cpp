#include "Node.h"

void Node::Start()
{
	m_active = false;
	m_lifetime = 0.0;
}

void Node::Update(double dt)
{
	if (m_active)
		m_lifetime += dt;
	else
		m_lifetime = 0;

	for (auto &childNode : m_children)	
		if (childNode.second->IsActive())
			childNode.second->Update(dt);
}

void Node::End()
{
	for (auto &childNode : m_children)
		childNode.second->End();
}

void Node::SetName(std::string name)
{
	m_name = name;
}

std::string Node::GetName()
{
	return m_name;
}

bool Node::IsActive()
{
	return m_active;
}

void Node::ActiveSelf(bool active)
{
	m_active = active;
}

double Node::GetTimeAlive()
{
	return m_lifetime;
}
