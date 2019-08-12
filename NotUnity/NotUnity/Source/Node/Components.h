#ifndef COMP_CONTAINER_H
#define COMP_CONTAINER_H

#include "Node.h"
#include "../Utility/TypeID.h"

//doesnt really do anything,just a container for Components

class CompContainer : public Node, public TypeID<CompContainer>
{
public:
	CompContainer(std::string name = "CompContainer"); 
	~CompContainer();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
};

#endif