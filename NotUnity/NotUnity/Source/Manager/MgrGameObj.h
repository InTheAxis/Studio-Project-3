#ifndef MGR_GO_H
#define MGR_GO_H

#include <map>
#include <vector>

#include "Manager.h"

class GameObj;
class Scene;
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

	void RegisterRenderable(GameObj* go);
	std::vector<GameObj*>* GetRenderables();
protected:
	MgrGameObj(std::string name = "MgrGameObj") : Manager<MgrGameObj>(name) {}
	~MgrGameObj() {}

	std::map<Scene*, std::map<std::string, GameObj*>> goList;

	std::vector<GameObj*> renderables;
};
#endif