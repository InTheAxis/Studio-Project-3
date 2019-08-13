#ifndef COMPONENT_BASE_H
#define COMPONENT_BASE_H

#include <ostream>
#include <typeinfo>

//basically this gives each component type a unique id, inherit from Component<T> to use
//avoid pointers, unless you overload operator= and copy constructor
//all components should inherit from Component<T> to get unique id
/********************************************
  EXAMPLE USAGE

class CompExample : public Component<CompExample>
{
public:
	CompExample() {}
	~CompExample() {}
	
	float myVar;
};
********************************************/

typedef unsigned long long ComponentID;

class GameObject;
class CompBase
{	
public:
	GameObject* parent;
	virtual CompBase* Clone() = 0;
protected:
	inline static ComponentID GetID() { static ComponentID m_incrementingID = 0; return m_incrementingID++; }
};

template<typename T>
class Component : public CompBase
{
public:
	static const ComponentID m_id;
	virtual CompBase* Clone() { T* ret = new T; *ret = *(reinterpret_cast<T*>(this)); return reinterpret_cast<CompBase*>(ret); }	
};

template<typename T>
const ComponentID Component<T>::m_id(CompBase::GetID());

//test comp below
class CompTransform : public Component<CompTransform>
{
public:
	CompTransform() {}
	~CompTransform() {}
	float temp;	
};

#endif