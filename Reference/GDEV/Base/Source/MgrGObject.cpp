#include "MgrGObject.h"

void MgrGObject::CloneComp(ComponentID id, GameObject * owner, GameObject * clone)
{
	//depends on overloaded '=' if there is pointers in component
	m_allComponents[id][clone] = m_allComponents[id][owner]->Clone();
}

std::map<GameObject*, CompBase*> MgrGObject::RetrieveCompList(ComponentID id)
{
	if (m_allComponents[id].empty())
		Debug::LogError("No such Component List!");
	return m_allComponents[id]; //return empty map
}
