#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Scripts.h"
#include "../Components.h"
#include "../../Utility/Delegate.h"
#include "../../Utility/Singleton.h"

class Achievements : public Node, public TypeID<Achievements>, public Component, public Singleton<Achievements>
{
	friend Singleton<Achievements>;
	friend Node;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	//make a bunch of delegates which are what the player will do to unlock achievements
	//in main scene or something else, attach the effect to the delegate
	//call the delegates when the thing happens i.e. PlayerDie when player dies

	Delegate<void> OnPlayerDie; //example, can attach any "void func()" to it

private:
	Achievements(std::string name = "Achievements");
	~Achievements();
};

#endif