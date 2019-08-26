#include "Achievements.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../Node/Components/KinemeticBody.h"
#include "../../Node/Scripts/PlayerController.h"

void Achievements::AchievementCheck()
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

int Achievements::GetAttacTimes(int at)
{
	attackTimes += at;
	return attackTimes;
}

bool Achievements::attackAch(bool AA)
{
	return AA;
}

bool Achievements::attackAch2(bool AA)
{
	return AA;
}

int Achievements::GetJumpTimes(int jt)
{
	jumpTimes += jt;
	return jumpTimes;
}

bool Achievements::jumpAch(bool JA)
{
	return JA;
}

bool Achievements::jumpAch2(bool JA)
{
	return JA;
}

double Achievements::GetWalkTime(double wt)
{
	walkTime += wt;
	return walkTime;
}

bool Achievements::walkAch(bool WA)
{
	return WA;
}

bool Achievements::walkAch2(bool WA)
{
	return WA;
}

int Achievements::GetEnemyKilled()
{
	return enemyKilled;
}
void Achievements::SetEnemyKilled(int ek)
{
	 enemyKilled += currentKilled;
	
}
void Achievements::SetCurrentEnemyKilled(int ek)
{
	currentKilled = ek;
	totalEnemyKilled = enemyKilled + currentKilled;
}
bool Achievements::enemyAch()
{
	if (totalEnemyKilled >= 10)
	{
		return true;
	}
	else
		return false;
}


void Achievements::ReadTextFile()
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

void Achievements::WriteTextFile()
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

void Achievements::setKnibRefrence(KinemeticBody * knib)
{
	knibReference = knib;
}

Achievements::Achievements(std::string name) : Node(name), maxValX(1), maxValY(1)
{
	
}

Achievements::~Achievements()
{
	
}

void Achievements::Start()
{			
	enemyDowned = false;
	ReadTextFile();
	Node::Start();
}

void Achievements::Update(double dt)
{
	//Debug::Log(enemyKilled);
	//Debug::Log(currentKilled);
	//Debug::Log(totalEnemyKilled);
	WriteTextFile();
	AchievementCheck();
	Node::Update(dt);
}

void Achievements::End()
{
	Node::End();
}
