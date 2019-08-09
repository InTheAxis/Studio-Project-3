#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include "Node.h"
#include "../Utility/TypeID.h"

class Scripts;
class Components;
class Transform;
class GameObj : public Node, public TypeID<GameObj>
{
public:
	GameObj(std::string name = "GameObj") : Node(name) {}
	~GameObj() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
private:
	Transform* t;
	Components* comps;
	Scripts* scripts;
};

#endif