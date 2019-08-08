#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include "Node.h"
#include "../Utility/TypeID.h"

class GameObj : public Node, public TypeID<GameObj>
{
public:
	GameObj(std::string name = "GameObj") : Node(name) {}
	~GameObj() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
};

#endif