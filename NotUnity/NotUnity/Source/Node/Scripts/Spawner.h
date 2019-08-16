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

class Spawner : public Node, public TypeID<Spawner>, public Component
{
public:
	Spawner(std::string name = "Spawner");
	~Spawner();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	void setWave(int waved); //Set starting wave from SpawnerScene

	//Init Enemies for different waves
	void EnemyWaveOne(std::string waveOne);
	void EnemyWaveTwo(std::string waveTwo);
	void EnemyWaveThree(std::string waveThree);
	void EnemyWaveFour(std::string waveFour);
	void EnemyWaveFive(std::string waveFive);

	void setEnemiesActive(std::string waves, int maxEneCount, float time); //Update enemies

private:
	int enemyCount; //Current enemies that are alive
	int MAX_ENEMY_COUNT; //Max number of enemies of that particular wave
	float interval; //Time interval per enemy spawn

	enum Wave //use to activate the enemies in that wave
	{
		DEFAULT,
		WAVEONE,
		WAVETWO,
		WAVETHREE,
		WAVEFOUR,
		WAVEFIVE,
	};

	int wave; //Current wave number
	std::string EnemyNames; //use to pass to and from different functions
	bool doneOne; //To state wave 1
};

#endif