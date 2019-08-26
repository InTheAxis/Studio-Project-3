#include "MgrAchievements.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../Node/Components/KinemeticBody.h"
#include "../../Node/Scripts/PlayerController.h"

MgrAchievements::MgrAchievements(std::string name)
	: Manager<MgrAchievements>(name)
	, maxValX(1)
	, maxValY(1)
{
}

MgrAchievements::~MgrAchievements()
{
}

void MgrAchievements::Start()
{
	enemyDowned = false;
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
	if (attackTimes >= 10)
	{
		attackAch(true);

		if (attackAch(true) && attackTimes >= 50)
			attackAch2(true);
	}
	if (jumpTimes >= 10)
	{
		if (knibReference->IsActive())
		{
			knibReference->maxVel.y = 1.3;
		}
		jumpAch(true);
		if (jumpAch(true) && jumpTimes >= 100)
		{
			if (knibReference->IsActive())
			{
				knibReference->maxVel.y = 1.6;
			}
		}
	}
	if (walkTime >= 10)
	{
		if (knibReference->IsActive())
		{
			knibReference->maxVel.x = 1.3;
		}
		walkAch(true);
		if (walkAch(true) && walkTime >= 200)
		{
			if (knibReference->IsActive())
			{
				knibReference->maxVel.x = 1.6;
			}
		}
	}

}

int MgrAchievements::GetAttacTimes(int at)
{
	attackTimes += at;
	return attackTimes;
}

bool MgrAchievements::attackAch(bool AA)
{
	return AA;
}

bool MgrAchievements::attackAch2(bool AA)
{
	return AA;
}

int MgrAchievements::GetJumpTimes(int jt)
{
	jumpTimes += jt;
	return jumpTimes;
}

bool MgrAchievements::jumpAch(bool JA)
{
	return JA;
}

bool MgrAchievements::jumpAch2(bool JA)
{
	return JA;
}

double MgrAchievements::GetWalkTime(double wt)
{
	walkTime += wt;
	return walkTime;
}

bool MgrAchievements::walkAch(bool WA)
{
	return WA;
}

bool MgrAchievements::walkAch2(bool WA)
{
	return WA;
}

int MgrAchievements::GetEnemyKilled()
{
	return enemyKilled;
}

void MgrAchievements::SetEnemyKilled(int ek)
{
	 enemyKilled += currentKilled;
	
}
void MgrAchievements::SetCurrentEnemyKilled(int ek)
{
	currentKilled = ek;
	totalEnemyKilled = enemyKilled + currentKilled;
}
bool MgrAchievements::enemyAch()
{
	if (totalEnemyKilled >= 10)
	{
		return true;
	}
	else
		return false;
}

void MgrAchievements::ReadTextFile()
{
	std::string line;
	std::ifstream Print("Resources/LifeTimeStats.txt");

	if (Print.is_open())
	{
		while (std::getline(Print,line))
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


			this->attackTimes = std::stoi(attack);
			this->jumpTimes = std::stoi(jump);
			this->walkTime = std::stoi(walkTimer);
			this->enemyKilled = std::stoi(enemyKILL);

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

		Write << attackTimes << "," << jumpTimes << "," << walkTime << "," << totalEnemyKilled;
		
	}
}

void MgrAchievements::setKnibRefrence(KinemeticBody * knib)
{
	knibReference = knib;
}