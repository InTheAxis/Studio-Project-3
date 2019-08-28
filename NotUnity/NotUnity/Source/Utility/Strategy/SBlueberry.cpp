#include "SBlueberry.h"
#include "../../NotUnity/Source/Node/Components/KinematicBody.h"
#include "../../NotUnity/Source/Node/Scripts/Projectile.h"

SBlueberry::SBlueberry() 
	: currentState(IDLE)
	, shouldAttack(false)
	, inteval(0.f)
	, atkIn(0.f)
	, speed(0)
	, boss(false)
{
}

SBlueberry::~SBlueberry()
{
}

void SBlueberry::Update(Vector3& dest, Vector3& enemyPos, KinematicBody* kb, double dt)
{
	shouldAttack = true;

	if ((dest - enemyPos).Length() < 4.f && (dest - enemyPos).Length() > 0.5f) //0.4f
		currentState = ATTACK;
	else if ((dest - enemyPos).Length() <= 0.5f)
		currentState = ATTACH;
	else
		currentState = TRAVEL;

	switch (currentState)
	{
	case ATTACK:
		kb->ApplyForce((dest - enemyPos).Normalized() * 0.2f);
		break;
	case ATTACH:
		enemyPos = dest;
		kb->ResetVel(1, 0);
		break;
	default: //TRAVEL
		kb->ApplyForce((dest - enemyPos).Normalized());
		break;
	}
}

void SBlueberry::Attack(Projectile* p, Vector3& enemyPos, Vector3& direction, double dt)
{
	inteval += 1.f * static_cast<float>(dt);

	if (boss)
		speed = 5.f;
	else
		speed = 6.f;

	if (shouldAttack && inteval >= speed)
	{
		if (p)
		{
			Vector3 temp(0, 0, 0);
			if (direction.Length() == 0)
				p->Discharge(enemyPos, direction* 5);
			else
				p->Discharge(enemyPos, direction.Normalize() * 5);
			p->GetGameObj()->ActiveSelf(true);
		}
		inteval = 0.f;
	}
}

bool SBlueberry::SelfInflict()
{
	return false;
}

bool SBlueberry::HasArmor()
{
	return false;
}

void SBlueberry::Boss(bool boss)
{
	this->boss = boss;
}

SBlueberry::CURRENT_STATE SBlueberry::GetState(void)
{
	return currentState;
}

void SBlueberry::SetState(SBlueberry::CURRENT_STATE state)
{
	currentState = state;
}
