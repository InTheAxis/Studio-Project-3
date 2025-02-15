#ifndef SKILL_H
#define SKILL_H

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Scripts.h"
#include "../Components.h"
#include "../../Utility/Delegate.h"

class Skill : public Node, public TypeID<Skill>, public Component
{
public:
	Skill(std::string name = "Skill");
	~Skill();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();		

	void Create(int cost, Skill* previous, bool unlocked); //create a skill, telling me the prev skill to unlock, and whether it starts out unlocked
	bool Unlock(int* skillPts); //give the func how many points you curr have, it'll minus for you
	bool GetUnlocked(); // get if skill is unlocked already 
	Skill* GetPrevious();
private:

	bool unlocked; //whether it is unlocked
	int cost; //cost of this skill
	Skill* previous; //prev skill to unlock to unlock this
};

#endif