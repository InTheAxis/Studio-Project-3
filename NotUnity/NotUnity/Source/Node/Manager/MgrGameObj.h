#ifndef MGR_GO_H
#define MGR_GO_H

#include <map>
#include <vector>

#include "Manager.h"

class Scene;
class GameObj;
class Renderable;
class MgrGameObj : public Manager<MgrGameObj>
{
	friend Singleton<MgrGameObj>;
	friend Node;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	GameObj* CreateGameObj(std::string name, Scene* ref);
	GameObj* GetGameObj(std::string name, Scene* ref);

	void RegisterRenderable(Renderable* go);
	std::vector<Renderable*>* GetRenderables();
protected:
	MgrGameObj(std::string name = "MgrGameObj");
	~MgrGameObj();

	std::map<Scene*, std::map<std::string, GameObj*>> goList;

	std::vector<Renderable*> renderables;
};
#endif