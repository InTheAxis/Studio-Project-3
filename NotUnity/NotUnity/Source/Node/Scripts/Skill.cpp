#include "Skill.h"

Skill::Skill(std::string name) 
	: Node(name)
	, unlocked(false)
	, cost(0)
	, previous(nullptr)
{
}

Skill::~Skill()
{
}

void Skill::Start()
{			
	Node::Start();
}

void Skill::Update(double dt)
{
	Node::Update(dt);
}

void Skill::End()
{
	Node::End();
}

void Skill::Create(int cost, Skill * previous = nullptr, bool unlocked = false)
{
	this->cost = cost;
	this->previous = previous;
	this->unlocked = unlocked;
}

bool Skill::Unlock(int* skillPts)
{
	if (*skillPts >= cost)
	{
		if (previous == nullptr || previous->unlocked)
		{
			*skillPts -= cost;
			unlocked = true;
			return true;
		}
	}

	return false;
}

bool Skill::GetUnlocked()
{
	return unlocked;
}

Skill * Skill::GetPrevious()
{
	return previous;
}
