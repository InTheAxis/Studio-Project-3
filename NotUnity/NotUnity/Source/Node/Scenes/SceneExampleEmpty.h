#ifndef EXAMPLE_SCENE_H
#define EXAMPLE_SCENE_H

#include "../Scene.h"

//plain scene to show how to inherit properly

class ExampleScene : public Scene //alright to inherit because I won't need to Get it
{
public:
	ExampleScene(std::string name = "ExampleScene");
	~ExampleScene();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void Render();
};

#endif