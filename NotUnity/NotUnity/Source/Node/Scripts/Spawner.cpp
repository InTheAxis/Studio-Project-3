#include "Spawner.h"
#include "../Manager/MgrGraphics.h"
#include "../../Node/GameObj.h"

Spawner::Spawner(std::string name)
	: Node(name)
	, wave(DEFAULT)
	, enemyCount(0)
	, interval(0.f)
	, poolCount(20)
	, EnemyNames("")
	, playerTrans(0.f, 0.f, 0.f)
	, enemyLeft(0)
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
		interval += 1.f * dt;
	else
		interval = 0;
	if (interval >= 1.5f)
		SpawnEnemy("e1");

	UpdatePlayerPosToAI("e1");

	//int left = MAX_ENEMY_COUNT - enemyCount;
	//Debug::Log("EC: " + std::to_string(enemyCount) + " | MEC: " + std::to_string(MAX_ENEMY_COUNT) + " | Left: " + std::to_string(left) + " | EC Name: " + EnemyNames);
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

//void Spawner::SetEnemiesActive(std::string names, int maxEneCount, float time)
//{
//	interval += 1.f * time;
//
//	for (int i = 0; i < maxEneCount; ++i)
//	{
//		if (interval >= 1.5f && enemyCount < 10 && enemyLeft > 0)
//		{
//			if (gameObject->GetChild<GameObj>(names + std::to_string(i))->IsActive())
//				continue;
//
//			gameObject->GetChild<GameObj>(names + std::to_string(i))->GetTransform()->translate = gameObject->GetTransform()->translate;
//			gameObject->GetChild<GameObj>(names + std::to_string(i))->ActiveSelf(true);
//			enemyCount++;
//			enemyLeft--;
//			interval = 0;
//			doneOne = true;
//		}
//	}
//	Debug::Log(std::to_string(enemyCount) + ", " + std::to_string(enemyLeft));
//}

void Spawner::SetPlayerTrans(Vector3 trans)
{
	playerTrans = trans;
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
	GameObj* go;
	for (unsigned int i = 0; i < poolCount; ++i)
	{
		go = gameObject->AddChild<GameObj>(waveOne + std::to_string(i));
		go->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("anim")); 
		go->ActiveSelf(false);
		go->AddScript<AI>()->SetHealth(10.f);
	}
}

void Spawner::SpawnEnemy(std::string waveOne)
{
	GameObj* go;
	Vector3 spawnerPos = gameObject->GetTransform()->translate;
	Vector3 offset;
	for (int i = 0; i < poolCount; ++i)
	{
		go = gameObject->GetChild<GameObj>(waveOne + std::to_string(i));
		if (go->IsActive())
			continue;

		int sign = (Math::RandIntMinMax(0, 1) * 2 - 1);
		offset.x = sign * (3 + Math::RandFloatMinMax(0, 2));

		sign = (Math::RandIntMinMax(0, 1) * 2 - 1);
		offset.y = sign * (1 + Math::RandFloatMinMax(0, 1));

		go->GetTransform()->translate = spawnerPos + offset;
		go->ActiveSelf(true);


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
