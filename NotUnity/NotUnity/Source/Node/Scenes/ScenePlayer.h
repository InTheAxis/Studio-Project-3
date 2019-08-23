#ifndef SCENE_PLAYER_H
#define SCENE_PLAYER_H

#include "../Scene.h"
#include "../../Utility/Math/Spline.h"
#include "../../Node/Scene.h"

class Player;
class PlayerController;
class Renderable;
class ScenePlayer : public Scene
{
public:
	ScenePlayer(std::string name = "ScenePlayer");
	~ScenePlayer();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	int GetHealth();
	GameObj* GetPlayer();
	ScenePlayer* SetTerrain(Spline* s);
	ScenePlayer* SetColorSpotRad(float radius);

	void Render();	
	
protected:
	PlayerController* playerScript;
};

#endif