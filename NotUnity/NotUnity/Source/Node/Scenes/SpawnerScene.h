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

class SpawnerScene : public Scene
{
public:
	SpawnerScene(std::string name = "SpawnerScene");
	~SpawnerScene();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void OnEnable();
	void OnDisable();

	void Render();
	void SetWave(int wave); //Set starting wave from other scene
	void PlayerTrans(Vector3 trans);
private:
	int currentWave;
	Vector3 fromPlayer;
	GameObj* SpawnerGO;
};

#endif