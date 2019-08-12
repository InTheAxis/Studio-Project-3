#ifndef COMPONENT_H
#define COMPONENT_H

class GameObj;
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