#ifndef SCENE_PLAYER_H
#define SCENE_PLAYER_H

#include "../Scene.h"

//plain scene to show how to inherit properly
class ColorSpot;
class ScenePlayer : public Scene  //alright to inherit because I won't need to Get it
{
	ColorSpot* colorSpot;
public:
	ScenePlayer(std::string name = "ScenePlayer");
	~ScenePlayer();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
	void SetY(float trans);

	void Render();
protected:
	float fromSpline;
};

#endif