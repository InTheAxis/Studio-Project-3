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
	//Delegate are call back pointers
	//Delegate<void> WalkTimer; // Walk for a certain amount of time and gain more movement speed
	//Delegate<void> OnJump; // Jump higher after jumping 10 time 
	//Delegate<void> OnAttack; // More Attack Damage after attacking 10 times
	//Delegate<void> OnEnemyKilled; // Kill 10 enemies and gain hp
	//Delegate<void> OnPlayerDie; //example, can attach any "void func()" to it

	void AchievementCheck();

	// Attacking 10 times gains you more damage
	int GetAttacTimes(int at);
	int attackTimes;
	bool attackAch;

	// Killing 10 enemies and gain hp
	int GetEnemyKilled();
	int enemyKilled;
	bool enemyAch;

	// Jumping 10 times gains you 
	int GetJumpTimes(int jt);
	int jumpTimes;
	bool jumpAch;

	double GetWalkTime(double wt);
	double walkTime;
	bool walkAch;
	
private:
	Achievements(std::string name = "Achievements");
	~Achievements();
};

#endif