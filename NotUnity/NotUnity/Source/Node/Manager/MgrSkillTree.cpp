#include "MgrSkillTree.h"

MgrSkillTree::MgrSkillTree(std::string name)
	: Manager<MgrSkillTree>(name)
{
}

MgrSkillTree::~MgrSkillTree()
{
}

void MgrSkillTree::Start()
{
	skills[0] = AddChild<Skill>("Skill1");
	skills[1] = AddChild<Skill>("Skill2");
	skills[2] = AddChild<Skill>("Skill3");
	Node::Start();
}

void MgrSkillTree::Update(double dt)
{
	Node::Update(dt);
}

void MgrSkillTree::End()
{
	Node::End();
}

void MgrSkillTree::ExpGained(int exp)
{
	expPoint += exp;
	while (expPoint >= 10)
	{
		expPoint -= 10;
		skillPoint++;
	}
	Debug::Log(expPoint);
	Debug::Log(skillPoint);
}

void MgrSkillTree::SpendSkillPoint(int level)
{
	if (skills[level]->GetPrevious()->GetUnlocked())
	{
		skills[level]->Unlock(&skillPoint);
	}
}

std::vector<int> MgrSkillTree::GetUnlockableSkill()
{
	std::vector<int> temp;
	if (skills[0]->GetUnlocked())
	{
		for (int i = 1; i < 3; ++i)
		{
			if (skills[i]->GetPrevious()->GetUnlocked())
				temp.emplace_back(i);
		}
	}
	else
		temp.emplace_back(0);
	return temp;
}
