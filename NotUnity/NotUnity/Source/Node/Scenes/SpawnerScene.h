#ifndef SPAWNER_SCENE_H
#define SPAWNER_SCENE_H

#include "../Scene.h"
#include "../Components/Sprite.h"
#include "../Components/ColliderRender.h"
#include "../../Utility/Math/Splines/Cubic.h"
#include "../../Utility/Math/Splines/Exponential.h"
#include "../../Utility/Math/Splines/Logarithm.h"
#include "../../Utility/Math/Splines/Quadratic.h"
#include "../../Utility/Math/Splines/Reciprocal.h"
#include "../../Utility/Math/Splines/SqReci.h"

//plain scene to show how to inherit properly

class SpawnerScene : public Scene //alright to inherit because I won't need to Get it
{
public:
	SpawnerScene(std::string name = "SpawnerScene");
	~SpawnerScene();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void Render();
	void setWave(int wave); //Set starting wave from other scene
private:

	int currentWave;
};

#endif