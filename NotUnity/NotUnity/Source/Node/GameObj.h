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

	template <typename T>
	T* AddComp(std::string key = std::to_string(T::m_id)) const
	{
		return comps->AddChild<T>(key);
	}
	template <typename T>
	T* AddScript(std::string key = std::to_string(T::m_id)) const
	{
		return scripts->AddChild<T>(key);
	}
	template <typename T>
	T* GetComp(std::string key = std::to_string(T::m_id)) const
	{
		return comps->GetChild<T>(key);
	}
	template <typename T>
	T* GetScript(std::string key = std::to_string(T::m_id)) const
	{
		return scripts->GetChild<T>(key);
	}
	Transform* GetTransform() const
	{
		return t;
	}

private:
	//refrences to children
	Components* comps;
	Scripts* scripts;
	Transform* t;
};

#endif