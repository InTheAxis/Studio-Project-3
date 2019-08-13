#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "WinDebug.h"

#include <vector>
#include <string>

#include "CompBase.h"
#include "MgrGObject.h"

//this class defines what a 'thing' is in our engine
//try to use unique names, as I don't check lol
//don't destroy gameobjects frequently, use their active state instead
//Transform() is a shortcut to getting transform component

class GameObject
{
public:
	GameObject();
	GameObject(std::string name);
	~GameObject();

	//duplicate this gameobject
	GameObject* Clone();

	//getter setters
	void SetName(std::string name);	
	std::string GetName();	
	void ActiveSelf(bool setActive);	
	bool IsActive();	
	CompTransform* Transform();
	static int GetCount(bool onlyActive);

	//getter setters for components
	template <typename T>
	T* GetComponent()
	{		
		return MgrGObject::Instance().RetrieveComp<T>(this);
	}
	template <typename T>
	T* AddComponent()
	{
		m_componentIds.emplace_back(T::m_id);
		return MgrGObject::Instance().RegisterComp<T>(this);
	}
private:
	std::string m_name;
	bool m_active;
	CompTransform* m_transform;	
	std::vector<ComponentID> m_componentIds; //to remember what components it has

	static int m_totalCount, m_activeCount; //how many gameobjects
};

#endif