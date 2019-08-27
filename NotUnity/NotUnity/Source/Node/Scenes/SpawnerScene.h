#ifndef SPAWNER_SCENE_H
#define SPAWNER_SCENE_H

#include "../Scene.h"
#include "../Components/Sprite.h"
#include "../../Utility/Math/Spline.h"
#include "../Components/Text.h"
#include <sstream>

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
	int GetWave();
	void PlayerTrans(Vector3 trans);
	void SetTerrain(Spline* s);
	void Reset();

	int GetEnemyKilled();
	int GetSpawnerWave();
	bool GetBossKilled();
	void NewWave();
	void SetStartGame(bool start);

private:
	Vector3 fromPlayer;
	GameObj* SpawnerGO;
	std::stringstream ss;
	Text* waveNum;
};

#endif