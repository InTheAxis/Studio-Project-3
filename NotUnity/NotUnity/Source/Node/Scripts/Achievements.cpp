#include "Achievements.h"

void Achievements::AchievementCheck()
{
	if (attackTimes >= 10)
	{
		attackAch = true;
	}
	if (jumpTimes >= 10)
	{
		jumpAch = true;
	}
	if (walkTime >= 10)
	{
		walkAch = true;
	}
}

int Achievements::GetAttacTimes(int at)
{
	attackTimes += at;
	return attackTimes;
	
}

int Achievements::GetJumpTimes(int jt)
{
	jumpTimes += jt;
	return jumpTimes;
}

double Achievements::GetWalkTime(double wt)
{
	walkTime += wt;
	return walkTime;
}

Achievements::Achievements(std::string name) : Node(name)
{
	
}

Achievements::~Achievements()
{
	
}

void Achievements::Start()
{			
	/*attackAch = false;
	walkTime = false;
	jumpTimes = false;*/

	
	Node::Start();
}

void Achievements::Update(double dt)
{
	Debug::Log(walkTime);
	AchievementCheck();
	Node::Update(dt);
}

void Achievements::End()
{
	Node::End();
}
