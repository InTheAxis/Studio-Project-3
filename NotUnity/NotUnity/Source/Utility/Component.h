#ifndef COMPONENT_H
#define COMPONENT_H

#include "../Node/GameObj.h"

class Component
{
public:
	Component() : gameObject(nullptr) {}
	~Component() {}
	
	GameObj* GetGameObj()
	{
		return gameObject;
	}
	void SetGameObj(GameObj* go)
	{
		gameObject = go;
	}

protected:
	GameObj* gameObject;
};


#endif