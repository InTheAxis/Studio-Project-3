#ifndef MGR_SCENE_H
#define MGR_SCENE_H

#include <string>
#include <map>

#include "Manager.h"

class NScene;
class MgrScene : public Manager<MgrScene>
{
	friend Singleton<MgrScene>;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
	int i = 0;
private:
	MgrScene(std::string name = "MgrScene") : Manager<MgrScene>(name) {}
	~MgrScene() {}

};
#endif