#ifndef STOMATO_H
#define STOMATO_H

#include <vector>
#include "Strategy.h"
#include "../Math/Vector3.h"

class KinemeticBody;
class Projectile;
class STomato : public Strategy
{
public:
	STomato();
	~STomato();

	void Update(Vector3& dest, Vector3& enemyPos, KinemeticBody* kb, double dt);
	void Attack(Projectile* p, Vector3& enemyPos, Vector3& direction, double dt);
	bool SelfInflict();
	bool HasArmor();
	void Boss(bool boss);

	enum CURRENT_STATE
	{
		ATTACK,
		IDLE,
	};

	STomato::CURRENT_STATE GetState();
	void SetState(STomato::CURRENT_STATE state);

private:
	STomato::CURRENT_STATE currentState;
	bool shouldAttack;
	float inteval;
	float speed;
	bool boss;
};
#endif