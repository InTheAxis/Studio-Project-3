#ifndef MGR_COMP_H
#define MGR_COMP_H

#include <map>
#include <vector>

#include "WinDebug.h"
#include "MgrBase.h"
#include "CompBase.h"
#include "ScriptBase.h"

//this manager stores all components and scripts
//this acts as the bridge between systems and gameobjects

class GameObject;
class MgrGObject : public Mgr<MgrGObject>
{
	friend class Singleton<MgrGObject>;	
public:
	virtual std::string Log(std::string name = "");
	
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
		comp->owner = owner;
		comp->enabled = true;

		ScriptBase* script = dynamic_cast<ScriptBase*>(comp);
		if (script)
		{
			m_allScripts[T::m_id][owner] = script;
		}

		m_allComponents[T::m_id][owner] = reinterpret_cast<CompBase*>(comp);
		return RetrieveComp<T>(owner);
	}

	void CloneComp(ComponentID id, GameObject* owner, GameObject* clone);
	std::map<GameObject*, CompBase*> RetrieveCompList(ComponentID id);

private:
	MgrGObject() {}
	~MgrGObject() {}
	std::map<ComponentID, std::map<GameObject*, CompBase*>> m_allComponents;
	std::map<ComponentID, std::map<GameObject*, ScriptBase*>> m_allScripts; //a sub-array from m_allComponents to easily loop through scripts
};

#endif