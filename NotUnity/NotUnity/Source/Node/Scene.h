#ifndef SCENE_H
#define SCENE_H

#include "Node.h"
#include "../Utility/TypeID.h"

class Scene : public Node, public TypeID<Scene>
{
public:
	Scene(std::string name = "Scene") : Node(name) {}
	~Scene() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	
};

#endif