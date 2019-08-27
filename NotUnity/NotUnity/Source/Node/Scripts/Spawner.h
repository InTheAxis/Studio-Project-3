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
#include "../Components/Text.h"
#include <sstream>

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
	void SetStartGame(bool start);
	void Cheat();

private:
	int enemyCount;
	int waveCount;
	float interval;
	bool startGame;

	int wave;
	int enemyType;
	Vector3 playerTrans;
	
	const int poolCount = 6;
	GameObj* enemyPool[6];
	GameObj* boss;

	//Init Enemies for different waves
	void CreateEnemies(std::string waveOne);
	void SpawnEnemy();
	void GetEnemyCount();

	void UpdateColorSpots();
	void UpdatePlayerPosToAI();
	void SetEnemyCount(int enemyCount);

	bool IsWaveDone();
	void CreateBoss(std::string bosStage);
	void SpawnBoss();
	void ChangeStrategy(Strategy* newStrategy, bool remove = true);

	Strategy* strategy;
	STomato sTomato;
	SCarrot sCarrot;
	SBanana sBanana;
	SKiwi sKiwi;
	SBlueberry sBlueberry;

	std::stringstream ss;
	Text* waveNum;
};

#endif