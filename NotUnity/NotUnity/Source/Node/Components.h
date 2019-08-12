#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "Node.h"
#include "../Utility/TypeID.h"

class Components : public Node, public TypeID<Components>
{
public:
	Components(std::string name = "Components"); 
	~Components();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
};

#endif