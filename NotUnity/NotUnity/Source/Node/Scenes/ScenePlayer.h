#ifndef SCENE_PLAYER_H
#define SCENE_PLAYER_H

#include "../Scene.h"
#include "../../Utility/Math/Spline.h"
#include "../../Node/Scene.h"

//plain scene to show how to inherit properly
class ColorSpot;
class Player;
class PlayerController;
class Renderable;
class ScenePlayer : public Scene  //alright to inherit because I won't need to Get it
{
public:
	ScenePlayer(std::string name = "ScenePlayer");
	~ScenePlayer();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	GameObj* GetPlayer();
	void SetTerrain(Spline* s);

	void Render();	
	
	int GetHealth();
private:
	//ScenePlayer* player;

	//Renderable* redbar;
	//Renderable* yellowbar;

	//GameObj* playerGO;

protected:

	ColorSpot* colorSpot;
	PlayerController* playerScript;
};

#endif