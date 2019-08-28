#ifndef STRATEGY_H
#define STRATEGY_H

#include <vector>
#include "../Math/Vector3.h"
#include "../../Node/Components/KinematicBody.h"
#include "../../Node/Scripts/Projectile.h"

class Strategy
{
public:
	Strategy();
	virtual ~Strategy();

	virtual void Update(Vector3& dest, Vector3& enemyPos, KinematicBody* kb, double dt) = 0;
	virtual void Attack(Projectile* p, Vector3& enemyPos, Vector3& direction, double dt) = 0;
	virtual bool SelfInflict() = 0;
	virtual bool HasArmor() = 0;
	virtual void Boss(bool boss) = 0;
};
#endif