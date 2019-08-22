#ifndef SPAWNER_SCENE_H
#define SPAWNER_SCENE_H

#include "../Scene.h"
#include "../Components/Sprite.h"
#include "../../Utility/Math/Spline.h"

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
	void SetTerrain(Spline* s);
	void Reset();

	int GetEnemyKilled();
private:
	int currentWave;
	Vector3 fromPlayer;
	GameObj* SpawnerGO;
};

#endif