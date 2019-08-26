#ifndef SPAWNER_H
#define SPAWNER_H

#include "../Node.h"
#include "../Scene.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Components.h"
#include "../Components/Sprite.h"
#include "../Scripts.h"
#include "../Scripts/AI.h"
#include "../../Utility/Math/Spline.h"

#include "../../Utility/Strategy/Strategy.h"
#include "../../Utility/Strategy/STomato.h"
#include "../../Utility/Strategy/SCarrot.h"
#include "../../Utility/Strategy/SBanana.h"
#include "../../Utility/Strategy/SKiwi.h"
#include "../../Utility/Strategy/SBlueberry.h"

class Spawner : public Node, public TypeID<Spawner>, public Component
{
public:
	Spawner(std::string name = "Spawner");
	~Spawner();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	void SetSpawnerWave(int waved);
	int GetSpawnerWave();
	void SetPlayerTrans(Vector3 trans);
	void SetTerrain(Spline* s);

	int GetEnemiesKilled();
	bool GetBossKilled();
	
	void Reset();
	void NewWave();
	void SetStrategy(int wave);

private:
	int enemyCount;
	int waveCount;
	float interval;

	int wave;
	std::string EnemyNames; 
	Vector3 playerTrans;

	int enemyLeft;
	
	const int poolCount = 20;
	GameObj* enemyPool[20];
	GameObj* boss;

	//Init Enemies for different waves
	void CreateEnemies(std::string waveOne);
	void SpawnEnemy(std::string waveOne);
	void GetEnemyCount(std::string waveOne);

	void UpdateColorSpots();
	void UpdatePlayerPosToAI(std::string names);
	void SetEnemyCount(int enemyCount);

	bool IsWaveDone();
	void CreateBoss(std::string bosStage);
	void SpawnBoss(std::string bosStage);

	Strategy* strategy;
	STomato sTomato;
	SCarrot sCarrot;
	SBanana sBanana;
	SKiwi sKiwi;
	SBlueberry sBlueberry;

};

#endif