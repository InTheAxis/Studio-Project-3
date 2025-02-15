#include "Node.h"

#include <sstream>

Node::Node(std::string name) 
	: m_name(name) 
	, m_active(true)
	, parent(nullptr)
	, m_newed(false)
{
}

Node::~Node()
{
	parent = nullptr;
	std::stringstream ss;
	for (auto &childNode : m_children)
	{
		if (childNode.second)
		{
			ss.str("");
			ss << "Deleted " << childNode.second->GetName() << " " << childNode.second;
			Debug::Log(ss.str());
			delete childNode.second;
		}
		childNode.second = nullptr;
	}
	m_children.clear();
}

void Node::Start()
{	
	m_lifetime = 0.0;

	for (auto &childNode : m_children)		
		childNode.second->Start();
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

void Node::OnEnable()
{
}

void Node::OnDisable()
{
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
	if (active) OnEnable();
	else OnDisable();
	for (auto &childNode : m_children)
		childNode.second->ActiveSelf(active);
}

double Node::GetTimeAlive()
{
	return m_lifetime;
}

Node * Node::GetParent()
{
	return parent;
}


