#include "Spawner.h"
#include "../Manager/MgrGraphics.h"
#include "../../Node/GameObj.h"

Spawner::Spawner(std::string name)
	: Node(name)
	, enemyCount(0)
	, interval(0.f)
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

	Node::Start();
}

void Spawner::Update(double dt)
{
	if (!wave) //== 0
		return;
	
	if (IsWaveDone())
		return;

	GetEnemyCount("e1");

	if (enemyCount < 10)
		interval += 1.f * static_cast<float>(dt);
	else
		interval = 0;
	if (interval >= 1.5f)
		SpawnEnemy("e1");

	UpdatePlayerPosToAI("e1");

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

void Spawner::SetPlayerTrans(Vector3 trans)
{
	playerTrans = trans;
}

void Spawner::SetTerrain(Spline * s)
{
	for (int i = 0; i < poolCount; ++i)
	{
		enemyPool[i]->GetScript<AI>()->SetTerrain(s);
	}
}

void Spawner::UpdatePlayerPosToAI(std::string names)
{
	for (int i = 0; i < poolCount; ++i)
	{
		if (gameObject->GetChild<GameObj>(names + std::to_string(i))->IsActive())
			gameObject->GetChild<GameObj>(names + std::to_string(i))->GetScript<AI>()->SetPlayerTrans(playerTrans);
	}
}

void Spawner::SetEnemyCount(int enemyCount)
{
	this->enemyCount = enemyCount;
}

bool Spawner::IsWaveDone()
{
	return waveCount > poolCount;
}

void Spawner::CreateEnemies(std::string waveOne)
{
	for (unsigned int i = 0; i < poolCount; ++i)
	{
		enemyPool[i] = gameObject->AddChild<GameObj>(waveOne + std::to_string(i));
		enemyPool[i]->ActiveSelf(false);
		enemyPool[i]->AddScript<AI>()->SetHealth(10.f);
		enemyPool[i]->GetScript<AI>()->ResetBullets();
	}
}

void Spawner::SpawnEnemy(std::string waveOne)
{
	Vector3 spawnerPos = gameObject->GetTransform()->translate;
	Vector3 offset;
	for (int i = 0; i < poolCount; ++i)
	{
		if (enemyPool[i]->IsActive())
			continue;

		int sign = (Math::RandIntMinMax(0, 1) * 2 - 1);
		offset.x = sign * (3 + Math::RandFloatMinMax(0, 2));

		offset.y = (1 + Math::RandFloatMinMax(0, 3));

		enemyPool[i]->GetTransform()->translate = spawnerPos + offset;
		enemyPool[i]->ActiveSelf(true);
		enemyPool[i]->GetScript<AI>()->ResetBullets();

		++waveCount;
		interval = 0;
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
