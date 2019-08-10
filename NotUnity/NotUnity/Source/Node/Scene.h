#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Node.h"
#include "../Utility/TypeID.h"

class Renderable;
class Scene : public Node, public TypeID<Scene>
{
public:
	Scene(std::string name = "Scene") : Node(name) {}
	~Scene() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void Render();

private:
	std::vector<Renderable*> renderables;	
};

#endif