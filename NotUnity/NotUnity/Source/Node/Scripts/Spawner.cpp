#include "Spawner.h"
#include "../Manager/MgrGraphics.h"
#include "../../Node/GameObj.h"
#include "../../Node/Scripts//ColorSpot.h"
#include "../Manager/MgrAchievements.h"

Spawner::Spawner(std::string name)
	: Node(name)
	, enemyCount(0)
	, interval(1.f)
	, EnemyNames("")
	, playerTrans(0.f, 0.f, 0.f)
	, enemyLeft(0)
	, waveCount(0)
	, wave(0)
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
	UpdateColorSpots();

	if (!wave) //== 0
		return;
	
	if (IsWaveDone())
		return;

	GetEnemyCount("e1");

	if (enemyCount < 10)
		interval += 1.f * static_cast<float>(dt);
	else
		interval = 0;

	if (interval >= 3.f)
	{
		SpawnEnemy("e1");
		interval = 0;
	}

	UpdatePlayerPosToAI("e1");

	if (GetEnemiesKilled() >= 3 && !GetBossKilled())
	{
		SpawnBoss("boss");
	}

	Node::Update(dt);
}

void Spawner::End()
{
	Node::End();
}

void Spawner::SetSpawnerWave(int waved)
{
	wave = waved;
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

void Spawner::UpdateColorSpots()
{
	for (int i = 0; i < poolCount; ++i)
		enemyPool[i]->GetScript<ColorSpot>()->SetUniform(i + 1); //cuz player reserves 0
	boss->GetScript<ColorSpot>()->SetUniform(poolCount + 1);
}

void Spawner::UpdatePlayerPosToAI(std::string names)
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
	boss->AddScript<AI>()->SetHealth(3);
	boss->AddScript<ColorSpot>();
	boss->GetScript<AI>()->ResetBullets();
	boss->GetTransform()->scale.Set(2.f, 2.f, 2.f);
}

void Spawner::SpawnBoss(std::string bosStage)
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
	boss->GetScript<AI>()->Reset();
	boss->GetTransform()->translate = spawnerPos + offset;
	//Debug::Log("BOSS: " + std::to_string(boss->GetScript<AI>()->GetHealth()));
}

void Spawner::CreateEnemies(std::string waveOne)
{
	for (unsigned int i = 0; i < poolCount; ++i)
	{
		enemyPool[i] = gameObject->AddChild<GameObj>(waveOne + std::to_string(i));
		enemyPool[i]->ActiveSelf(false);
		enemyPool[i]->AddScript<AI>()->SetHealth(3);
		enemyPool[i]->AddScript<ColorSpot>();		
	}
}

void Spawner::SpawnEnemy(std::string waveOne)
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
		enemyPool[i]->GetScript<AI>()->Reset();		
		enemyPool[i]->GetTransform()->translate = spawnerPos + offset;
		//Debug::Log("ENEMY: " + std::to_string(enemyPool[i]->GetScript<AI>()->GetHealth()));
		++waveCount;
		return;
	}
}

void Spawner::GetEnemyCount(std::string waveOne)
{
	enemyCount = 0;
	for (int i = 0; i < poolCount; ++i)
	{
		if (gameObject->GetChild<GameObj>(waveOne + std::to_string(i))->IsActive())
			++enemyCount;
	}
}
