#ifndef SKILL_TREE_H
#define SKILL_TREE_H

#include <vector>

#include "Manager.h"
#include "../../Utility/Delegate.h"
#include "../Scripts/Skill.h"

class MgrSkillTree : public Manager<MgrSkillTree>
{
	friend Singleton<MgrSkillTree>;
	friend Node;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();		
	
	void ExpGained(int exp);//func to earn exp
	void SpendSkillPoint(int level);//func to spend skill pt to unlock given skill
	std::vector<int> GetUnlockableSkill();//func to get current unlockable skills

private:
	MgrSkillTree(std::string name = "MgrSkillTree");
	~MgrSkillTree();

	
	int expPoint;  //some exp tracker , every expPoint 10 is skillPoint
	int skillPoint;//amount of skill points

	Skill* skills[3];
};

#endif