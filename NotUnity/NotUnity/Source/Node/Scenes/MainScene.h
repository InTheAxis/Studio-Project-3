#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "../Scene.h"

#include "../../Utility/Graphics/FBO/FloatFBO.h"
#include "ScenePlayer.h"
#include "SpawnerScene.h"

class SpawnerScene;
class ScenePlayer;
class MapScene;

class MainScene : public Scene //alright to inherit because I won't need to Get it
{
public:
	MainScene(std::string name = "MainScene");
	~MainScene();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void Render();
private:
	FloatFBO floatFbo[2];
	SpawnerScene* sceneSpawner;
	ScenePlayer* scenePlayer;
	MapScene* map;

	bool debug;
};

#endif