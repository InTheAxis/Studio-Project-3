#ifndef COMPONENT_H
#define COMPONENT_H

class GameObj;
class CompBase
{
public:
	CompBase() : gameObject(nullptr) {}
	~CompBase() {}
	
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