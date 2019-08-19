#ifndef SKILL_TREE_H
#define SKILL_TREE_H
#include <vector>
#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Scripts.h"
#include "../Components.h"
#include "../../Utility/Delegate.h"
#include "../../Utility/Singleton.h"
#include "Skill.h"

class SkillTree : public Node, public TypeID<SkillTree>, public Component, public Singleton<SkillTree>
{
	friend Singleton<SkillTree>;
	friend Node;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();		
	
	void ExpGained(int exp);//func to earn exp
	void SpendSkillPoint(int level);//func to spend skill pt to unlock given skill
	std::vector<int> GetUnlockableSkill();//func to get current unlockable skills

private:
	SkillTree(std::string name = "SkillTree");
	~SkillTree();

	
	int expPoint;  //some exp tracker , every expPoint 10 is skillPoint
	int skillPoint;//amount of skill points

	Skill* skills[3];
};

#endif