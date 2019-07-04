#ifndef COMPONENT_BASE_H
#define COMPONENT_BASE_H

//basically this gives each component type a unique id, inherit from Component to use

typedef unsigned long long ComponentID;

class GameObject;
class CompBase
{	
public:
	GameObject* parent;
protected:
	inline static ComponentID GetID() { static ComponentID m_incrementingID = 0; return m_incrementingID++; }	
};

template<typename T>
class Component : public CompBase
{
public:
	static const ComponentID m_id;
};

template<typename T>
const ComponentID Component<T>::m_id(CompBase::GetID());
#endif