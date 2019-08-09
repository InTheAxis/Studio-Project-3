#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include "Node.h"
#include "../Utility/TypeID.h"
#include "Components.h"
#include "Scripts.h"

class Transform;
class GameObj : public Node, public TypeID<GameObj>
{
public:
	GameObj(std::string name = "GameObj") : Node(name) {}
	~GameObj() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	GameObj* Create(std::string filePath);
	GameObj* Create(Components::PRESET cPreset = Components::EMPTY, Scripts::PRESET sPreset = Scripts::EMPTY);

	template <typename T>
	T* GetComp(std::string key = std::to_string(T::m_id))
	{
		return comps->GetChild<T>(key);
	}
	template <typename T>
	T* GetScripts(std::string key = std::to_string(T::m_id))
	{
		return scripts->GetChild<T>(key);
	}

private:
	//refrences to children
	Components* comps;
	Scripts* scripts;
	Transform* t;
};

#endif