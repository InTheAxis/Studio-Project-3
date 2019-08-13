#include "GameObject.h"

int GameObject::m_totalCount = 0, GameObject::m_activeCount = 0;

GameObject::GameObject()
{
	m_name = "GameObject" + std::to_string(m_totalCount);
	this->m_transform = AddComponent<CompTransform>();
	this->ActiveSelf(true);
	++m_totalCount;
}

GameObject::GameObject(std::string name)
{
	m_name = name;
	this->m_transform = AddComponent<CompTransform>();
	this->ActiveSelf(false);
	++m_totalCount;
}

GameObject::~GameObject()
{
	--m_totalCount;
}

GameObject* GameObject::Clone()
{
	GameObject* copy = new GameObject(this->m_name + "Copy");
	//manual copy to ensure it is deep
	copy->m_active = m_active;
	copy->m_transform = m_transform;
	copy->m_componentIds = m_componentIds;
	for (auto id : m_componentIds)
	{
		MgrGObject::Instance().CloneComp(id, this, copy);
	}
	return copy;
}

void GameObject::SetName(std::string name)
{
	m_name = name;
}

std::string GameObject::GetName()
{
	return m_name;
}

void GameObject::ActiveSelf(bool setActive)
{
	if (setActive && !m_active)
		++m_activeCount;
	else if (!setActive && m_active)
		--m_activeCount;
	m_active = setActive;
}

bool GameObject::IsActive()
{
	return m_active;
}

CompTransform * GameObject::Transform()
{
	return m_transform;
}

int GameObject::GetCount(bool onlyActive)
{
	if (onlyActive)
		return m_activeCount;
	else
		return m_totalCount;
}
