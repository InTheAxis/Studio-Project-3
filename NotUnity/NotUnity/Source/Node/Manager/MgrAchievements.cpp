#include "MgrAchievements.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../Node/Components/KinemeticBody.h"
#include "../../Node/Scripts/PlayerController.h"

MgrAchievements::MgrAchievements(std::string name)
	: Manager<MgrAchievements>(name)
	, walkTime(0)
	, maxValX1(1.3)
	, maxValX2(1.6)
	, walkAchievementLevel1(false)
	, walkAchievementLevel2(false)
	, bonusHealth(0)
{
}

MgrAchievements::~MgrAchievements()
{
}

void MgrAchievements::HardReset()
{
	attackTimes = 0;
	jumpTimes = 0;
	walkTime = 0;
	enemyKilled = 0;
}

void MgrAchievements::Start()
{
	ReadTextFile();
	Node::Start();
}

void MgrAchievements::Update(double dt)
{
	//Debug::Log(enemyKilled);
	//Debug::Log(currentKilled);
	//Debug::Log(totalEnemyKilled);
	WriteTextFile();
	AchievementCheck();
	Node::Update(dt);
}

void MgrAchievements::End()
{
	Node::End();
}

void MgrAchievements::AchievementCheck()
{
	if (attackTimes >= 10 && !attackAchievementLevel1)
	{
		attackAchievementLevel1 = true;
	}
	if (attackTimes >= 50 && !attackAchievementLevel2)
	{
		attackAchievementLevel2 = true;
	}

	//if (jumpTimes >= 10)
	//{
	//	if (knibReference->IsActive())
	//	{
	//		knibReference->maxVel.y = 1.3;
	//	}
	//	GetJumpLevel1(true);
	//	if (GetJumpLevel1(true) && jumpTimes >= 100)
	//	{
	//		if (knibReference->IsActive())
	//		{
	//			knibReference->maxVel.y = 1.6;
	//		}
	//	}
	//}

	if (walkTime >= 10 && !walkAchievementLevel1)
	{
		walkAchievementLevel1 = true;
		if (knibReference->IsActive())
			knibReference->maxVel.x = 1.3f;
		else
			Debug::Log("KNIBREFERENCE IS NOT ACTIVE!");
	}
	if (walkTime >= 100 && !walkAchievementLevel2)
	{
		walkAchievementLevel2 = true;
		if (knibReference->IsActive())
			knibReference->maxVel.x = 1.6f;
		else
			Debug::Log("KNIBREFERENCE IS NOT ACTIVE!");
	}

	if (finalEnemyKilled >= 10 && !enemyKilledAchievementLevel1)
	{
		enemyKilledAchievementLevel1 = true;
		bonusHealth = 10;
	}
	if (finalEnemyKilled >= 100 && !enemyKilledAchievementLevel2)
	{
		enemyKilledAchievementLevel2 = true;
		bonusHealth = 50;
	}
}

void MgrAchievements::SetAttackTimes(int at)
{
	attackTimes += at;
}

int MgrAchievements::GetAttactTimes()
{
	return attackTimes;
}

bool MgrAchievements::GetAttackLevel1()
{
	return attackAchievementLevel1;
}

bool MgrAchievements::GetAttackLevel2()
{
	return attackAchievementLevel2;
}

void MgrAchievements::SetJumpTimes(int jt)
{
	jumpTimes += jt;
}

int MgrAchievements::GetJumpTimes(int jt)
{
	return jumpTimes;
}

bool MgrAchievements::GetJumpLevel1()
{
	return jumpAchievementLevel1;
}

bool MgrAchievements::GetJumpLevel2()
{
	return jumpAchievementLevel2;
}

void MgrAchievements::SetWalkTime(float wt)
{
	walkTime += wt;
}

float MgrAchievements::GetWalkTime()
{
	return walkTime;
}

bool MgrAchievements::GetWalkAchievementLevel1()
{
	return walkAchievementLevel1;
}

bool MgrAchievements::GetWalkAchievementLevel2()
{
	return walkAchievementLevel2;
}

int MgrAchievements::GetEnemyKilled()
{
	return enemyKilled;
}

void MgrAchievements::SetEnemyKilled(int ek)
{
	if (enemyKilled < ek)
	{
		enemyKilled = ek;
	}
}

void MgrAchievements::SetTotalKilled(int tk)
{
	totalEnemyKilled += tk;
}

void MgrAchievements::SetFinalKilled()
{
	finalEnemyKilled = totalEnemyKilled + enemyKilled;
	totalEnemyKilled = 0;
	enemyKilled = 0;
}

void MgrAchievements::ResetEnemyKilled()
{
	finalEnemyKilled = totalEnemyKilled + enemyKilled;
	enemyKilled = 0;
}

int MgrAchievements::GetFinalEnemyKilled()
{
	return finalEnemyKilled;
}

int MgrAchievements::GetBonusHealth()
{
	return bonusHealth;
}

bool MgrAchievements::GetKillLevel1()
{
	return enemyKilledAchievementLevel1;
}

bool MgrAchievements::GetKillLevel2()
{
	return walkAchievementLevel2;
}

void MgrAchievements::ReadTextFile()
{
	std::string line;
	std::ifstream Print("Resources/LifeTimeStats.txt");

	if (Print.is_open())
	{
		while (std::getline(Print, line))
		{
			int ID = line.find(","); //find ,
			std::string tmp = line.substr(ID + 1);
			int Cord = tmp.find(","); //cord after this
			std::string tmp2 = tmp.substr(Cord + 1);
			int Coord = tmp2.find(",");
			std::string tmp3 = tmp2.substr(Coord + 1);

			std::string attack = line.substr(0, ID);
			Debug::Log(line);
			Debug::Log(tmp);
			Debug::Log(tmp2);
			Debug::Log(tmp3);
			std::string jump = tmp.substr(0, Cord);

			std::string walkTimer = tmp2.substr(0, Coord);

			std::string enemyKILL = tmp3;

			this->attackTimes = std::stoi(attack);
			this->jumpTimes = std::stoi(jump);
			this->walkTime = std::stof(walkTimer);
			this->finalEnemyKilled = std::stoi(enemyKILL);
		}
		Print.close();
	}
}

void MgrAchievements::WriteTextFile()
{
	std::string line;
	std::ofstream Write("Resources/LifeTimeStats.txt");
	if (Write.is_open())
	{
		int ID = line.find(","); //find ,
		std::string tmp = line.substr(ID + 1);
		int Cord = tmp.find(","); //cord after this
		std::string tmp2 = tmp.substr(Cord + 1);
		int Coord = tmp2.find(",");
		std::string tmp3 = tmp2.substr(Coord + 1);

		std::string attack = line.substr(0, ID);
		std::string jump = tmp.substr(0, Cord);
		std::string walkTimer = tmp2.substr(Coord + 1);
		std::string enemyKILL = tmp3;

		Write << attackTimes << "," << jumpTimes << "," << walkTime << "," << finalEnemyKilled;
	}
}

void MgrAchievements::setKnibRefrence(KinemeticBody * knib)
{
	knibReference = knib;
}