#ifndef SBLUEBERRY_H
#define SBLUEBERRY_H

#include <vector>
#include "Strategy.h"
#include "../Math/Vector3.h"

class KinemeticBody;
class Projectile;
class SBlueberry : public Strategy
{
public:
	SBlueberry();
	~SBlueberry();

	void Update(Vector3& dest, Vector3& enemyPos, KinemeticBody* kb, double dt);
	void Attack(Projectile* p, Vector3& enemyPos, Vector3& direction, double dt);
	bool SelfInflict();
	bool HasArmor();
	void Boss(bool boss);

	enum CURRENT_STATE
	{
		ATTACK,
		TRAVEL,
		ATTACH,
		IDLE,
	};

	SBlueberry::CURRENT_STATE GetState();
	void SetState(SBlueberry::CURRENT_STATE state);

private:
	SBlueberry::CURRENT_STATE currentState;
	bool shouldAttack;
	float inteval;
	float atkIn;
	float speed;
	bool boss;
};
#endif