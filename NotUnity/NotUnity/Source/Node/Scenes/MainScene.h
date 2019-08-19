#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "../Scene.h"

#include "../../Utility/Graphics/FBO/FloatFBO.h"
#include "ScenePlayer.h"
#include "SpawnerScene.h"
#include "../../Utility/Math/Splines/Cubic.h"

class SpawnerScene;
class ScenePlayer;
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
	SplineCubic* spCubic;
	float spc;
};

#endif