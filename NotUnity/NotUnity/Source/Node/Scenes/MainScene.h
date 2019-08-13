#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "../Scene.h"

class MainScene : public Scene //alright to inherit because I won't need to Get it
{
public:
	MainScene(std::string name = "MainScene");
	~MainScene();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void Render();
};

#endif