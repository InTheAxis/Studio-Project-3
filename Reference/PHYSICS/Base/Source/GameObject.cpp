#include "GameObject.h"

int GameObject::count = 0;

GameObject::GameObject()
	: m_active(false)
	, m_transform(new CompTransform)	
	, m_tag(-999)
	, parent(nullptr)	
{
	for (auto pair : m_components)
	{
		pair.second = nullptr;
	}	
	AddComponent<CompTransform>(m_transform);
}

GameObject::~GameObject()
{
}

void GameObject::SetTag(unsigned long long ull)
{
	m_tag = static_cast<long long>(ull);
}

CompTransform * GameObject::Transform()
{
	return m_transform;
}

void GameObject::ActiveSelf(bool setActive)
{
	if (setActive && !m_active)
		count++;
	else if (!setActive && m_active)
		count = Math::Max(0, count - 1);
		
	m_active = setActive;
}

bool GameObject::IsActive()
{
	return m_active;
}