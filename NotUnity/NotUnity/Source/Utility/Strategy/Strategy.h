#ifndef STRATEGY_H
#define STRATEGY_H

#include <vector>
#include "../Math/Vector3.h"
#include "../../NotUnity/Source/Node/Components/KinemeticBody.h"
#include "../../NotUnity/Source/Node/Scripts/Projectile.h"

class Strategy
{
public:
	Strategy();
	~Strategy();

	virtual void Update(Vector3& dest, Vector3& enemyPos, KinemeticBody* kb, double dt) = 0;
	virtual void Attack(Projectile* p, Vector3& enemyPos, Vector3& direction, double dt) = 0;
	virtual bool SelfInflict() = 0;
	virtual bool HasArmor() = 0;
	virtual void Boss(bool boss) = 0;
};
#endif