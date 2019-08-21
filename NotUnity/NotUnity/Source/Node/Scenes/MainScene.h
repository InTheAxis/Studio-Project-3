#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "../Scene.h"

#include "../../Utility/Graphics/FBO/FloatFBO.h"

class SpawnerScene;
class ScenePlayer;
class MapScene;
class Sprite;
class Renderable;
class MainScene : public Scene //alright to inherit because I won't need to Get it
{
	enum GAME_STATE
	{
		MENU,
		TUTO,
		GAMEPLAY,
		LOSE,
		WIN
	};
public:
	MainScene(std::string name = "MainScene");
	~MainScene();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void Render();
private:
	FloatFBO floatFbo[2];

	//refrences
	SpawnerScene* spawner;
	ScenePlayer* player;
	MapScene* map;
	Sprite* title;
	Renderable* wasd;
	GameObj* playerGO;
	GameObj* mainCam;

	bool debug;
	GAME_STATE gs;

	double winTimer;

	void ChangeGameState(GAME_STATE gs);
};

#endif