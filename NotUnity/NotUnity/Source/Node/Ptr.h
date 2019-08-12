#ifndef PTR_H
#define PTR_H

#include "Node.h"
#include "../Utility/TypeID.h"

class Ptr : public Node, public TypeID<Ptr>
{
public:
	Ptr(std::string name = "Ptr"); 
	~Ptr();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	Node* ref;
};

#endif