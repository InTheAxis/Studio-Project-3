#ifndef N_GAMEOBJ_H
#define N_GAMEOBJ_H

#include "Node.h"
#include "../Utility/TypeID.h"

class NGameObj : public Node, public TypeID<NGameObj>
{
public:
	NGameObj() {}
	~NGameObj() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
};

#endif