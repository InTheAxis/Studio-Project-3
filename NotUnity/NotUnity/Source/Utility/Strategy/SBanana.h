#ifndef SBANANA_H
#define SBANANA_H

#include <vector>
#include "Strategy.h"
#include "../Math/Vector3.h"

class KinematicBody;
class Projectile;
class SBanana : public Strategy
{
public:
	SBanana();
	~SBanana();

	void Update(Vector3& dest, Vector3& enemyPos, KinematicBody* kb, double dt);
	void Attack(Projectile* p, Vector3& enemyPos, Vector3& direction, double dt);
	bool SelfInflict();
	bool HasArmor();
	void Boss(bool boss);

	enum CURRENT_STATE
	{
		ATTACK,
		WAIT,
		REPEL,
		IDLE,
	};

	SBanana::CURRENT_STATE GetState();
	void SetState(SBanana::CURRENT_STATE state);

private:
	SBanana::CURRENT_STATE currentState;
	bool shouldAttack;
	float inteval;
	float atkIn;
	bool selfInflict;
	float speed;
	bool boss;
};
#endif