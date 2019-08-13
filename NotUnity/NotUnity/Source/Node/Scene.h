#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Node.h"
#include "../Utility/TypeID.h"

class GameObj;
class Renderable;
class MgrGameObj;
class MgrGraphics;
class Scene : public Node, public TypeID<Scene>
{
public:
	Scene(std::string name = "Scene");
	~Scene();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	virtual void Render();

protected:
	MgrGraphics* mg;
	MgrGameObj* mgo;

	std::vector<Renderable*>* renderables;
};

#endif