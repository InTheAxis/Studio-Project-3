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

class Spawner : public Node, public TypeID<Spawner>, public Component
{
public:
	Spawner(std::string name = "Spawner");
	~Spawner();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	void SetSpawnerWave(int waved);
	void SetPlayerTrans(Vector3 trans);
	void SetTerrain(Spline* s);

	int GetEnemiesKilled();
	bool GetBossKilled();
	
	void Reset();

private:
	int enemyCount;
	int waveCount;
	float interval;

	int wave;
	std::string EnemyNames; 
	Vector3 playerTrans;

	int enemyLeft;
	bool canSpawn;
	
	const int poolCount = 20;
	GameObj* enemyPool[20];

	//Init Enemies for different waves
	void CreateEnemies(std::string waveOne);
	void SpawnEnemy(std::string waveOne);
	void GetEnemyCount(std::string waveOne);

	void UpdatePlayerPosToAI(std::string names);
	void SetEnemyCount(int enemyCount);

	bool IsWaveDone();
};

#endif