#include "Spawner.h"
#include "../Manager/MgrGraphics.h"
#include "../../Node/GameObj.h"

Spawner::Spawner(std::string name) : Node(name), wave(DEFAULT), enemyCount(0), 
interval(0.f), MAX_ENEMY_COUNT(0), EnemyNames(""), doneOne(false)
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
	if (enemyCount <= 0) //Need edit to fit the boss
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

	setEnemiesActive(EnemyNames, MAX_ENEMY_COUNT, static_cast<float>(dt));
	//std::cout << "Max EC " << MAX_ENEMY_COUNT << " CEC " << enemyCount << " Interval " << interval << std::endl;

	Node::Update(dt);
}

void Spawner::End()
{
	Node::End();
}

void Spawner::setWave(int waved)
{
	wave = waved;
}

void Spawner::setEnemiesActive(std::string waves, int maxEneCount, float time)
{
	interval += 1.f * time;
	for (int i = 0; i < maxEneCount; ++i)
	{
		if (interval >= 1.5f && enemyCount < 10)
		{
			if (gameObject->GetChild<GameObj>(waves + std::to_string(i))->IsActive())
				continue;

			gameObject->GetChild<GameObj>(waves + std::to_string(i))->ActiveSelf(true);
			enemyCount++;
			interval = 0;
			doneOne = true;
		}
	}
}

void Spawner::EnemyWaveOne(std::string waveOne)
{
	for (unsigned int i = 0; i < 20; ++i)
	{
		gameObject->AddChild<GameObj>(waveOne + std::to_string(i))->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("anim"));
		gameObject->GetChild<GameObj>(waveOne + std::to_string(i))->AddScript<AI>()->setHealth(10);
		gameObject->GetChild<GameObj>(waveOne + std::to_string(i))->GetTransform()->translate.Set(1 + i, 1, 0);
		gameObject->GetChild<GameObj>(waveOne + std::to_string(i))->ActiveSelf(false);
	}
}

void Spawner::EnemyWaveTwo(std::string waveTwo)
{
	for (unsigned int i = 0; i < 25; ++i)
	{
		gameObject->AddChild<GameObj>(waveTwo + std::to_string(i))->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("anim"));
		gameObject->GetChild<GameObj>(waveTwo + std::to_string(i))->AddScript<AI>()->setHealth(10);
		gameObject->GetChild<GameObj>(waveTwo + std::to_string(i))->GetTransform()->translate.Set(1 + i, 1, 0);
		gameObject->GetChild<GameObj>(waveTwo + std::to_string(i))->ActiveSelf(false);
	}
}

void Spawner::EnemyWaveThree(std::string waveThree)
{
	for (unsigned int i = 0; i < 30; ++i)
	{
		gameObject->AddChild<GameObj>(waveThree + std::to_string(i))->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("anim"));
		gameObject->GetChild<GameObj>(waveThree + std::to_string(i))->AddScript<AI>()->setHealth(10);
		gameObject->GetChild<GameObj>(waveThree + std::to_string(i))->GetTransform()->translate.Set(1 + i, 1, 0);
		gameObject->GetChild<GameObj>(waveThree + std::to_string(i))->ActiveSelf(false);
	}
}

void Spawner::EnemyWaveFour(std::string waveFour)
{
	for (unsigned int i = 0; i < 35; ++i)
	{
		gameObject->AddChild<GameObj>(waveFour + std::to_string(i))->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("anim"));
		gameObject->GetChild<GameObj>(waveFour + std::to_string(i))->AddScript<AI>()->setHealth(10);;
		gameObject->GetChild<GameObj>(waveFour + std::to_string(i))->GetTransform()->translate.Set(1 + i, 1, 0);
		gameObject->GetChild<GameObj>(waveFour + std::to_string(i))->ActiveSelf(false);
	}
}

void Spawner::EnemyWaveFive(std::string waveFive)
{
	for (unsigned int i = 0; i < 40; ++i)
	{
		gameObject->AddChild<GameObj>(waveFive + std::to_string(i))->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("anim"));
		gameObject->GetChild<GameObj>(waveFive + std::to_string(i))->AddScript<AI>()->setHealth(10);
		gameObject->GetChild<GameObj>(waveFive + std::to_string(i))->GetTransform()->translate.Set(1 + i, 1, 0);
		gameObject->GetChild<GameObj>(waveFive + std::to_string(i))->ActiveSelf(false);
	}
}
