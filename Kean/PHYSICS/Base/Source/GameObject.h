#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <map>
#include <vector>
#include "ComponentBase.h"
#include "CompTransform.h"

class GameObject
{	
public:
	GameObject();
	~GameObject();

	static int count;

	//getter setters for components
	template <typename T>
	T* GetComponent()
	{
		if (m_components.count(T::m_id) <= 0) //if no key
			return nullptr;
		return reinterpret_cast<T*>(m_components[T::m_id]);
	}
	template <typename T>
	T* AddComponent(T* comp = nullptr)
	{
		if (m_components[T::m_id] == nullptr)
		{
			if (comp == nullptr)
				comp = new T;

			comp->parent = this;

			m_components[T::m_id] = reinterpret_cast<CompBase*>(comp);
		}		

		return GetComponent<T>();
	}

	//setter for tag
	void SetTag(unsigned long long ull);
	//getter for transform component
	CompTransform* Transform();
	//setter for active state
	void ActiveSelf(bool setActive);
	//getter for active state
	bool IsActive();
private:
	std::map<ComponentID, CompBase*> m_components;	
	CompTransform* m_transform;
	bool m_active;
	long long m_tag;
};

#endif