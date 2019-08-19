#include "Spawner.h"
#include "../Manager/MgrGraphics.h"
#include "../../Node/GameObj.h"

Spawner::Spawner(std::string name) : Node(name), wave(DEFAULT), enemyCount(0), 
interval(0.f), MAX_ENEMY_COUNT(0), EnemyNames(""), doneOne(false), playerTrans(0.f, 0.f, 0.f)
{
}

Spawner::~Spawner()
{
}

void Spawner::Start()
{
	EnemyWaveOne("EnemiesOne");
	EnemyWaveTwo("EnemiesTwo");
	EnemyWaveThree("EnemiesThree");
	EnemyWaveFour("EnemiesFour");
	EnemyWaveFive("EnemiesFive");

	Node::Start();
}

void Spawner::Update(double dt)
{
	if (enemyCount == 0) //Need edit to fit the boss
	{
		if (doneOne)
			++wave;

		switch (wave)
		{
		case Spawner::WAVEONE:
			MAX_ENEMY_COUNT = 20;
			EnemyNames = "EnemiesOne";
			break;
		case Spawner::WAVETWO:
			MAX_ENEMY_COUNT = 25;
			EnemyNames = "EnemiesTwo";
			break;
		case Spawner::WAVETHREE:
			MAX_ENEMY_COUNT = 30;
			EnemyNames = "EnemiesThree";
			break;
		case Spawner::WAVEFOUR:
			MAX_ENEMY_COUNT = 35;
			EnemyNames = "EnemiesFour";
			break;
		case Spawner::WAVEFIVE:
			MAX_ENEMY_COUNT = 40;
			EnemyNames = "EnemiesFive";
			break;
		default:
			break;
		}
	}

	SetEnemiesActive(EnemyNames, MAX_ENEMY_COUNT, static_cast<float>(dt));
	AIMove(EnemyNames, MAX_ENEMY_COUNT);
	int left = MAX_ENEMY_COUNT - enemyCount;
	std::cout << "EC: " << enemyCount << " | MEC: " << MAX_ENEMY_COUNT << " | Left: " << left << " | EC Name: "<< EnemyNames  << std::endl;

	Node::Update(dt);
}

void Spawner::End()
{
	Node::End();
}

void Spawner::SetWave(int waved)
{
	wave = waved;
}

void Spawner::SetEnemiesActive(std::string names, int maxEneCount, float time)
{
	interval += 1.f * time;
	for (int i = 0; i < maxEneCount; ++i)
	{
		if (interval >= 1.5f && enemyCount < 10)
		{
			if (gameObject->GetChild<GameObj>(names + std::to_string(i))->IsActive())
				continue;

			gameObject->GetChild<GameObj>(names + std::to_string(i))->GetTransform()->translate = gameObject->GetTransform()->translate;
			gameObject->GetChild<GameObj>(names + std::to_string(i))->ActiveSelf(true);
			enemyCount++;
			interval = 0;
			doneOne = true;
		}
	}
}

void Spawner::SetPlayerTrans(Vector3 trans)
{
	playerTrans = trans;
}

void Spawner::AIMove(std::string names, int maxEneCount)
{
	for (int i = 0; i < maxEneCount; ++i)
	{
		if (gameObject->GetChild<GameObj>(names + std::to_string(i))->IsActive())
			gameObject->GetChild<GameObj>(names + std::to_string(i))->GetScript<AI>()->SetPlayerTrans(playerTrans);
	}
}

int Spawner::GetEnemyCount()
{
	return enemyCount;
}

void Spawner::SetEnemyCount(int enemyCount)
{
	this->enemyCount = enemyCount;
}

void Spawner::EnemyWaveOne(std::string waveOne)
{
	for (unsigned int i = 0; i < 20; ++i)
	{
		gameObject->AddChild<GameObj>(waveOne + std::to_string(i))->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("anim"));
		gameObject->GetChild<GameObj>(waveOne + std::to_string(i))->GetTransform()->translate.Set(1 + i, 1, 0);
		gameObject->GetChild<GameObj>(waveOne + std::to_string(i))->ActiveSelf(false);
		gameObject->GetChild<GameObj>(waveOne + std::to_string(i))->AddScript<AI>()->SetHealth(10.f); //Health 10, atkSpeed 1, damage 1
	}
}

void Spawner::EnemyWaveTwo(std::string waveTwo)
{
	for (unsigned int i = 0; i < 25; ++i)
	{
		gameObject->AddChild<GameObj>(waveTwo + std::to_string(i))->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("anim"));
		gameObject->GetChild<GameObj>(waveTwo + std::to_string(i))->GetTransform()->translate.Set(1 + i, 1, 0);
		gameObject->GetChild<GameObj>(waveTwo + std::to_string(i))->ActiveSelf(false);
		gameObject->GetChild<GameObj>(waveTwo + std::to_string(i))->AddScript<AI>()->SetHealth(12.5f); //Health 12.5, atkSpeed 1.1, damage 1.1
	}
}

void Spawner::EnemyWaveThree(std::string waveThree)
{
	for (unsigned int i = 0; i < 30; ++i)
	{
		gameObject->AddChild<GameObj>(waveThree + std::to_string(i))->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("anim"));
		gameObject->GetChild<GameObj>(waveThree + std::to_string(i))->GetTransform()->translate.Set(1 + i, 1, 0);
		gameObject->GetChild<GameObj>(waveThree + std::to_string(i))->ActiveSelf(false);
		gameObject->GetChild<GameObj>(waveThree + std::to_string(i))->AddScript<AI>()->SetHealth(15.6f); //Health 15.6, atkSpeed 1.2, damage 1.2
	}
}

void Spawner::EnemyWaveFour(std::string waveFour)
{
	for (unsigned int i = 0; i < 35; ++i)
	{
		gameObject->AddChild<GameObj>(waveFour + std::to_string(i))->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("anim"));
		gameObject->GetChild<GameObj>(waveFour + std::to_string(i))->GetTransform()->translate.Set(1 + i, 1, 0);
		gameObject->GetChild<GameObj>(waveFour + std::to_string(i))->ActiveSelf(false);
		gameObject->GetChild<GameObj>(waveFour + std::to_string(i))->AddScript<AI>()->SetHealth(19.5f); //Health 19.5, atkSpeed 1.3, damage 1.3
	}
}

void Spawner::EnemyWaveFive(std::string waveFive)
{
	for (unsigned int i = 0; i < 40; ++i)
	{
		gameObject->AddChild<GameObj>(waveFive + std::to_string(i))->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("anim"));
		gameObject->GetChild<GameObj>(waveFive + std::to_string(i))->GetTransform()->translate.Set(1 + i, 1, 0);
		gameObject->GetChild<GameObj>(waveFive + std::to_string(i))->ActiveSelf(false);
		gameObject->GetChild<GameObj>(waveFive + std::to_string(i))->AddScript<AI>()->SetHealth(25.f); //Health 25, atkSpeed 1.4, damage 1.4
	}
}
