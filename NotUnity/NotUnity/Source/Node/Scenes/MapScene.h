#ifndef MAP_SCENE_H
#define MAP_SCENE_H

#include "../Scene.h"
#include "../Components/Camera.h"

//plain scene to show how to inherit properly

class MapScene : public Scene //alright to inherit because I won't need to Get it
{
public:
	MapScene(std::string name = "MapScene");
	~MapScene();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void Render();
	void setCamera(Camera* camera);
private:
	Camera* camera;
};

#endif