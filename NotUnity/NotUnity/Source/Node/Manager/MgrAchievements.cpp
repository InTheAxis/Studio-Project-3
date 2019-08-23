#include "MgrAchievements.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../Node/Components/KinemeticBody.h"

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
	ReadTextFile();
	Node::Start();
}

void MgrAchievements::Update(double dt)
{
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
		if (jumpAch(true) && jumpTimes >= 50)
			jumpAch2(true);
	}
	if (walkTime >= 10)
	{
		if (knibReference->IsActive())
		{
			knibReference->maxVel.x = 1.3;
		}
		walkAch(true);
		if (walkAch(true) && walkTime >= 200)
			walkAch2(true);
	}
	if (enemyKilled >= 10)
	{
		enemyAch(true);
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
	return false;
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

int MgrAchievements::GetEnemyKilled(int ek)
{
	enemyKilled += ek;
	return enemyKilled;
}

bool MgrAchievements::enemyAch(bool EA)
{
	return EA;
}

void MgrAchievements::ReadTextFile()
{
	std::string line;
	std::ifstream Print("Resources/LifeTimeStats.txt");

	if (Print.is_open())
	{
		while (/*std::getline(stats , line)*/std::getline(Print,line))
		{
			int ID = line.find(","); //find , finds ,
			std::string tmp = line.substr(ID + 1);
			int Cord = tmp.find(","); //cord after this
			std::string attack = line.substr(0, ID);
			std::string jump = tmp.substr(0, Cord);
			std::string walkTimer = tmp.substr(Cord + 1);
			std::string enemyKILL = tmp.substr(Cord + 1);
				 


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
		int ID = line.find(",");
		std::string tmp = line.substr(ID + 1);
		int Cord = tmp.find(",");
		std::string attack = line.substr(0, ID);
		std::string jump = tmp.substr(0, Cord);
		std::string walkTimer = tmp.substr(Cord + 1);
		std::string enemyKILL = tmp.substr(Cord + 1);

		Write << attackTimes << "," << jumpTimes << "," << walkTime << "," << enemyKilled;
		
	}
}

void MgrAchievements::setKnibRefrence(KinemeticBody * knib)
{
	knibReference = knib;
}