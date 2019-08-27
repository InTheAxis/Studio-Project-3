#ifndef SCARROT_H
#define SCARROT_H

#include <vector>
#include "Strategy.h"
#include "../Math/Vector3.h"

class KinematicBody;
class Projectile;
class SCarrot : public Strategy
{
public:
	SCarrot();
	~SCarrot();

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

	SCarrot::CURRENT_STATE GetState();
	void SetState(SCarrot::CURRENT_STATE state);

private:
	SCarrot::CURRENT_STATE currentState;
	bool shouldAttack;
	float inteval;
	float atkIn;
	float randSpeed;
	float speed;
	bool boss;
};
#endif