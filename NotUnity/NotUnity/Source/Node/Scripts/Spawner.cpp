#include "Spawner.h"
#include "../Manager/MgrGraphics.h"
#include "../../Node/GameObj.h"
#include "../../Node/Scripts//ColorSpot.h"
#include "../Manager/MgrAchievements.h"
#include "../../Utility/Input/ControllerKeyboard.h"

Spawner::Spawner(std::string name)
	: Node(name)
	, enemyCount(0)
	, interval(1.f)
	, playerTrans(0.f, 0.f, 0.f)
	, waveCount(0)
	, wave(0)
	, startGame(false)
	, eNames("")
{
}

Spawner::~Spawner()
{
}

void Spawner::Start()
{
	CreateEnemies("e1");
	CreateBoss("boss");
	Node::Start();
}

void Spawner::Update(double dt)
{
	if (!startGame)
		return;

	UpdateColorSpots();
	
	if (IsWaveDone())
		return;

	GetEnemyCount();

	if (enemyCount < 10)
		interval += 1.f * static_cast<float>(dt);
	else
		interval = 0;

	if (interval >= 3.f)
	{
		SpawnEnemy();
		interval = 0;
	}

	UpdatePlayerPosToAI(); //Will update Boss too

	if (GetEnemiesKilled() >= 20 && !GetBossKilled()) //&& !GetBossKilled()
		SpawnBoss();

	Cheat();
	Node::Update(dt);
}

void Spawner::End()
{
	Node::End();
}

void Spawner::SetSpawnerWave(int wave)
{
	this->wave = wave;
}

int Spawner::GetSpawnerWave()
{
	return wave;
}

void Spawner::SetPlayerTrans(Vector3 trans)
{
	playerTrans = trans;
}

void Spawner::SetTerrain(Spline * s)
{
	for (int i = 0; i < poolCount; ++i)
	{
		enemyPool[i]->GetScript<AI>()->SetTerrain(s);
		boss->GetScript<AI>()->SetTerrain(s);
	}	
}

int Spawner::GetEnemiesKilled()
{
	int c = 0;
	for (int i = 0; i < poolCount; ++i)
	{
		if (enemyPool[i]->GetScript<AI>()->IsDead())
			++c;
	}
	MgrAchievements::Instance()->SetCurrentEnemyKilled(c);
	return c;
}

bool Spawner::GetBossKilled()
{
	if (boss->GetScript<AI>()->IsDead())
		return true;
	else
		return false;
}

void Spawner::Reset()
{
	for (int i = 0; i < poolCount; ++i)
	{
		enemyPool[i]->ActiveSelf(false);
		enemyPool[i]->GetScript<AI>()->Reset();
	}
	boss->ActiveSelf(false);
	boss->GetScript<AI>()->Reset();
	waveCount = enemyCount = 0;	
}

void Spawner::NewWave()
{
	for (int i = 0; i < poolCount; ++i)
	{
		enemyPool[i]->ActiveSelf(false);
		enemyPool[i]->GetScript<AI>()->Reset();
	}
	boss->ActiveSelf(false);
	boss->GetScript<AI>()->Reset();
	enemyCount = 0;
}

void Spawner::SetStrategy(int wave)
{
	if (strategy == NULL || strategy != NULL)
	{
		if (wave == 1)
		{
			strategy = &sTomato;
			ChangeStrategy(&sTomato, false);
			eNames = "enemy1";
		}
		else if (wave == 2)
		{
			strategy = &sCarrot;
			ChangeStrategy(&sCarrot, false);
			eNames = "enemy2";
		}
		else if (wave == 3)
		{
			strategy = &sBanana;
			ChangeStrategy(&sBanana, false);
			eNames = "enemy3";
		}
		else if (wave == 4)
		{
			strategy = &sKiwi;
			ChangeStrategy(&sKiwi, false);
			eNames = "enemy4";
		}
		else if (wave == 5)
		{
			strategy = &sBlueberry;
			ChangeStrategy(&sBlueberry, false);
			eNames = "enemy5";
		}
	}
}

void Spawner::SetStartGame(bool start)
{
	startGame = start;
}

void Spawner::Cheat()
{
	ControllerKeyboard* kb = ControllerKeyboard::Instance();
	
	if (kb->IsKeyDown('K'))
	{
		for (unsigned int i = 0; i < poolCount; ++i)
		{
			enemyPool[i]->GetScript<AI>()->SetDead(true);
		}
	}
}

void Spawner::CreateEnemies(std::string waveOne)
{
	Debug::Log(wave);
	for (unsigned int i = 0; i < poolCount; ++i)
	{
		enemyPool[i] = gameObject->AddChild<GameObj>(waveOne + std::to_string(i));
		enemyPool[i]->ActiveSelf(false);
		enemyPool[i]->AddScript<AI>()->SetHealth(3);
	}
}

void Spawner::SpawnEnemy()
{
	Vector3 spawnerPos = gameObject->GetTransform()->translate;
	Vector3 offset;
	for (int i = 0; i < poolCount; ++i)
	{
		if (enemyPool[i]->IsActive() || enemyPool[i]->GetScript<AI>()->IsDead())
			continue;

		int sign = (Math::RandIntMinMax(0, 1) * 2 - 1);
		offset.x = sign * (1 + Math::RandFloatMinMax(0, 2));
		offset.y = (playerTrans.y + Math::RandFloatMinMax(5, 7));
		offset.z = 1;

		enemyPool[i]->ActiveSelf(true);
		enemyPool[i]->GetScript<AI>()->SetStrategy(strategy);
		enemyPool[i]->GetScript<AI>()->SetName(eNames);
		enemyPool[i]->GetScript<AI>()->Reset();
		enemyPool[i]->GetTransform()->translate = spawnerPos + offset;
		++waveCount;
		return;
	}
}

void Spawner::GetEnemyCount()
{
	enemyCount = 0;
	for (int i = 0; i < poolCount; ++i)
	{
		//if (gameObject->GetChild<GameObj>(waveOne + std::to_string(i))->IsActive())
		if (enemyPool[i]->IsActive())
			++enemyCount;
	}
}

void Spawner::UpdateColorSpots()
{
	for (int i = 0; i < poolCount; ++i)
		enemyPool[i]->GetScript<AI>()->GetColorSpot()->SetUniform(i + 1); //cuz player reserves 0
	boss->GetScript<AI>()->GetColorSpot()->SetUniform(poolCount + 1);
}

void Spawner::UpdatePlayerPosToAI()
{
	for (int i = 0; i < poolCount; ++i)
		if (enemyPool[i]->IsActive())
			enemyPool[i]->GetScript<AI>()->SetPlayerTrans(playerTrans);

	if (boss->IsActive())
		boss->GetScript<AI>()->SetPlayerTrans(playerTrans);
}

void Spawner::SetEnemyCount(int enemyCount)
{
	this->enemyCount = enemyCount;
}

bool Spawner::IsWaveDone()
{
	return waveCount > poolCount;
}

void Spawner::CreateBoss(std::string bosStage)
{
	boss = gameObject->AddChild<GameObj>(bosStage);
	boss->ActiveSelf(false);
	boss->AddScript<AI>()->SetHealth(6);
	boss->GetTransform()->scale.Set(2.f, 2.f, 2.f);
}

void Spawner::SpawnBoss()
{
	Vector3 spawnerPos = gameObject->GetTransform()->translate;
	Vector3 offset;

	if (boss->IsActive())
		return;

	int sign = (Math::RandIntMinMax(0, 1) * 2 - 1);
	offset.x = sign * (1 + Math::RandFloatMinMax(0, 2));
	offset.y = (playerTrans.y + Math::RandFloatMinMax(5, 7));
	offset.z = 1;

	boss->ActiveSelf(true);
	boss->GetScript<AI>()->SetStrategy(strategy);
	boss->GetScript<AI>()->SetName(eNames);
	boss->GetScript<AI>()->Reset();
	boss->GetTransform()->translate = spawnerPos + offset;
}

void Spawner::ChangeStrategy(Strategy* newStrategy, bool remove)
{
	if (remove)
	{
		if (strategy != NULL)
		{
			delete strategy;
			strategy = NULL;
		}
	}
	strategy = newStrategy;
}