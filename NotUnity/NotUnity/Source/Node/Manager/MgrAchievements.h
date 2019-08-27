#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include "Manager.h"
#include "../../Utility/Delegate.h"

class KinemeticBody;
class MgrAchievements : public Manager<MgrAchievements>
{
	friend Singleton<MgrAchievements>;
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
	void SetAttackTimes(int at);
	int GetAttactTimes();
	bool GetAttackLevel1();
	bool GetAttackLevel2();

	// Jumping 10 times gains you
	void SetJumpTimes(int jt);
	int GetJumpTimes(int jt);
	bool GetJumpLevel1();
	bool GetJumpLevel2();

	//Get walk time
	void SetWalkTime(float wt); // increase over desire amount +=
	float GetWalkTime();
	bool GetWalkAchievementLevel1();
	bool GetWalkAchievementLevel2();

	// Killing 10 enemies and gain hp
	int GetEnemyKilled();
	void SetEnemyKilled(int ek);
	void SetTotalKilled(int tk);
	void SetFinalKilled();
	void ResetEnemyKilled();
	int GetFinalEnemyKilled();
	int GetBonusHealth();
	bool GetKillLevel1();
	bool GetKillLevel2();

	void ReadTextFile();
	void WriteTextFile();
	void setKnibRefrence(KinemeticBody* knib);
private:
	MgrAchievements(std::string name = "MgrAchievements");
	~MgrAchievements();

	// Walk achievement
	float walkTime;
	float maxValX1;
	float maxValX2;
	bool walkAchievementLevel1;
	bool walkAchievementLevel2;

	// attack Achievement
	int attackTimes;
	bool attackAchievementLevel1;
	bool attackAchievementLevel2;

	// Jump Achievement
	int jumpTimes;
	bool jumpAchievementLevel1;
	bool jumpAchievementLevel2;

	// Enemy killed Achievement
	int enemyKilled;
	int totalEnemyKilled;
	int finalEnemyKilled;
	bool enemyKilledAchievementLevel1;
	bool enemyKilledAchievementLevel2;
	int bonusHealth;

	void HardReset();

	KinemeticBody* knibReference; // get set private variable !!!
};

#endif