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
}

void Node::End()
{
	for (auto &childNode : m_children)
		childNode.second->End();
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
