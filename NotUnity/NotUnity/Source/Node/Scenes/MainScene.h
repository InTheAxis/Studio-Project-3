#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include <sstream>

#include "../Scene.h"

#include "../../Utility/Graphics/FBO/FloatFBO.h"
#include "../Components/Text.h"

class SpawnerScene;
class ScenePlayer;
class MapScene;
class Sprite;
class Renderable;
class Text;
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
	Sprite* walkAchievement;
	Sprite* attackAchievemnt;
	Sprite* killAchievement;
	Renderable* wasd;
	Renderable* lmb;
	Renderable* pauseMenu;

	Text* textWalkAchievement;
	Text* textAttackAchievement;
	Text* textJumpAchievement;
	Text* waveNum;
	std::stringstream ss;

	Renderable* greenbar;
	Renderable* redbar;
	//PlayerController* pc;

	GameObj* playerGO;
	GameObj* mainCam;

	bool debug;
	GAME_STATE gs;

	bool pause;
	double winTimer;
	float lightAngle;

	int healthminus;
	void ChangeGameState(GAME_STATE gs);

	float walkSat;
	float attSat;
	float killSat;

	void updateAchievementUI();
};

#endif