#ifndef SKILL_TREE_H
#define SKILL_TREE_H

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Scripts.h"
#include "../Components.h"
#include "../../Utility/Delegate.h"
#include "../../Utility/Singleton.h"

class SkillTree : public Node, public TypeID<SkillTree>, public Component, public Singleton<SkillTree>
{
	friend Singleton<SkillTree>;
	friend Node;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();		

	//func to earn exp
	//func to spend skill pt to unlock given skill
	//func to get current unlockable skills

private:
	SkillTree(std::string name = "SkillTree");
	~SkillTree();

	//some exp tracker
	//amount of skill points
	//use a node list of skills to represent skill tree
};

#endif