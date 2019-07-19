#ifndef N_SCENE_H
#define N_SCENE_H

#include "Node.h"
#include "../Utility/TypeID.h"

class NScene : public Node, public TypeID<NScene>
{
public:
	NScene(std::string name = "NScene") : Node(name) {}
	~NScene() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	
};

#endif