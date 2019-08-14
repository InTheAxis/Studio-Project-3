#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Node.h"
#include "../Utility/TypeID.h"

class GameObj;
class MgrGameObj;
class MgrGraphics;
enum class RENDER_PASS;
class Scene : public Node, public TypeID<Scene>
{
public:
	Scene(std::string name = "Scene");
	~Scene();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	virtual void Render();

	void RenderPass(RENDER_PASS rp);

protected:
	MgrGraphics* mg;
	MgrGameObj* mgo;	
};

#endif