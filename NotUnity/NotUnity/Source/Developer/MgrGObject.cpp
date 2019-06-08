#include "MgrGObject.h"

void MgrGObject::CloneComp(ComponentID id, GameObject * owner, GameObject * clone)
{
	m_allComponents[id][clone] = m_allComponents[id][owner]->Clone();
}

std::map<GameObject*, CompBase*> MgrGObject::RetrieveCompList(ComponentID id)
{
	if (m_allComponents[id].empty())
		Debug::LogError("No such Component List!");
	return m_allComponents[id]; //return empty map
}

std::string MgrGObject::Log(std::string name)
{
	std::stringstream ss;
	ss << "Logging state of Mgr" << name << "\n";	
	int count = 0, total = 0;
	for (auto &compList : m_allComponents)
	{
		ss << "   " << "ID" << compList.first << " Components: \n";
		ss << "    " << "Addresses: ";
		count = 0;
		for (auto &comp : compList.second)
		{
			ss << comp.second << " ";
			count++;
			total++;
		}
		ss << "\n    " << "Count: " << count << "\n";
	}
	ss << "\n  " << "Total Component Count: " << total << "\n\n";
	total = 0;
	for (auto &scriptList : m_allScripts)
	{
		ss << "   " << "ID" << scriptList.first << " Scripts: \n";
		ss << "    " << "Addresses: ";
		count = 0;
		for (auto &script : scriptList.second)
		{
			ss << script.second << " ";
			total++;
			count++;
		}
		ss << "\n    " << "Count: " << count << "\n";
	}
	ss << "\n  " << "Total Script Count: " << total << "\n";
	return ss.str();
}