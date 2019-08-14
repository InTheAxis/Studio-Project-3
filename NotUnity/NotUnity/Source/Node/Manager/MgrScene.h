#ifndef MGR_SCENE_H
#define MGR_SCENE_H

#include <map>
#include <string>

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

	Scene* GetRoot() const;	
	Scene* SwitchScene(std::string name);
protected:
	MgrScene(std::string name = "MgrScene"); 
	~MgrScene();

	Scene* rootScene;

	std::map<std::string, Scene*> allScenes;
	
	void LoadScenes();
};
#endif