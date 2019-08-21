#ifndef MAP_SCENE_H
#define MAP_SCENE_H

#include "../Scene.h"
#include "../Components/Camera.h"

//plain scene to show how to inherit properly

class MapGenerator;
class Spline;
class MapScene : public Scene //alright to inherit because I won't need to Get it
{
public:
	MapScene(std::string name = "MapScene");
	~MapScene();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void Render();
	void SetCamera(Camera* camera);
	Spline* GetTerrain();

	void ChangeToSaturated();
	void ChangeToDeSat();

private:
	Camera* camera;
	MapGenerator* mapGen;
};

#endif