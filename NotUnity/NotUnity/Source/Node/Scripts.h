#ifndef SCRIPTS_H
#define SCRIPTS_H

#include "Node.h"
#include "../Utility/TypeID.h"

class Scripts : public Node, public TypeID<Scripts>
{
public:
	Scripts(std::string name = "Scripts") : Node(name) {}
	~Scripts() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
};

#endif