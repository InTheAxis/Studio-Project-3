#ifndef MGR_SCENE_H
#define MGR_SCENE_H

#include <map>

#include "Manager.h"

class Scene;
class MgrScene : public Manager<MgrScene>
{
	friend Singleton<MgrScene>;
	friend Node;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	
protected:
	MgrScene(std::string name = "MgrScene") : Manager<MgrScene>(name) {}
	~MgrScene() {}

};
#endif