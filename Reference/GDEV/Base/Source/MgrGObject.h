#ifndef MGR_COMP_H
#define MGR_COMP_H

#include <map>
#include <vector>

#include "WinDebug.h"
#include "Singleton.h"
#include "CompBase.h"

//this manager stores all components and scripts
//this acts as the bridge between systems and gameobjects

class GameObject;
class MgrGObject : public Singleton<MgrGObject>
{
	friend class Singleton<MgrGObject>;	
public:
	
	//set and get components from list
	template <typename T>
	T* RetrieveComp(GameObject* owner)
	{
		if (m_allComponents[T::m_id].count(owner) <= 0) //if no key
		{
			Debug::LogError("Gameobject doesn't have this Component!");
			return nullptr;
		}
		return reinterpret_cast<T*>(m_allComponents[T::m_id][owner]);
	}

	template <typename T>
	T* RegisterComp(GameObject* owner)
	{		
		if (m_allComponents[T::m_id].count(owner) > 0)
		{
			Debug::LogWarning("GameObject already has this Component!");
			return RetrieveComp<T>(owner);
		}

		T* comp = new T;
		comp->parent = owner;

		m_allComponents[T::m_id][owner] = reinterpret_cast<CompBase*>(comp);
		return RetrieveComp<T>(owner);
	}

	void CloneComp(ComponentID id, GameObject* owner, GameObject* clone);
	std::map<GameObject*, CompBase*> RetrieveCompList(ComponentID id);

private:
	MgrGObject() {}
	~MgrGObject() {}
	std::map<ComponentID, std::map<GameObject*, CompBase*>> m_allComponents;
};

#endif