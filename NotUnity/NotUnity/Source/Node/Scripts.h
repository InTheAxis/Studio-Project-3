#ifndef SCRIPT_CONTAINER_H
#define SCRIPT_CONTAINER_H

#include "Node.h"
#include "../Utility/TypeID.h"

//doesnt really do anything,just a container for Components that i call Scripts

class ScriptContainer : public Node, public TypeID<ScriptContainer>
{
public:
	ScriptContainer(std::string name = "ScriptContainer");
	~ScriptContainer();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
};

#endif