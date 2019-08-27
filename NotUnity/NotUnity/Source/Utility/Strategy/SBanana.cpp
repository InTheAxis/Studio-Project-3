#include "SBanana.h"
#include "../../NotUnity/Source/Node/Components/KinemeticBody.h"
#include "../../NotUnity/Source/Node/Scripts/Projectile.h"

SBanana::SBanana()
	: currentState(IDLE)
	, shouldAttack(false)
	, inteval(0.f)
	, atkIn(0.f)
	, selfInflict(false)
	, speed(0)
	, boss(false)
{
}

SBanana::~SBanana()
{
}

void SBanana::Update(Vector3& dest, Vector3& enemyPos, KinemeticBody* kb, double dt)
{
	atkIn += 1.f * static_cast<float>(dt);
	shouldAttack = true;
	
	if (atkIn >= 3.2f)
	{
		if ((dest - enemyPos).LengthSquared() < 2.5f)
			currentState = REPEL;

		if ((dest - enemyPos).LengthSquared() > 6.f)
			currentState = WAIT;
	}
	else
	{
		if ((dest - enemyPos).LengthSquared() >= 2.f)
			currentState = ATTACK;
		else
			currentState = IDLE;
	}

	switch(currentState)
	{
	case ATTACK:
		kb->ApplyForce((dest - enemyPos).Normalized());
		break;
	case REPEL:
		kb->ApplyForce(-(dest - enemyPos).Normalized());
		break;
	case WAIT:
		kb->ResetVel(1, 0);
		atkIn = 0;
		break;
	default: //IDLE
		kb->ResetVel(1, 0);
		break;
	}
}

void SBanana::Attack(Projectile* p, Vector3& enemyPos, Vector3& direction, double dt)
{
	inteval += 1.f * static_cast<float>(dt);

	if (boss)
		speed = 3.5f;
	else
		speed = 4.f;

	if (shouldAttack && inteval >= speed)
	{
		if (p)
		{
			p->Discharge(enemyPos, direction.Normalized() * 10);
			p->GetGameObj()->ActiveSelf(true);
			selfInflict = true;
		}
		inteval = 0;
	}
	else
		selfInflict = false;
}

bool SBanana::SelfInflict()
{
	return selfInflict;
}

bool SBanana::HasArmor()
{
	return false;
}

void SBanana::Boss(bool boss)
{
	this->boss = boss;
}

SBanana::CURRENT_STATE SBanana::GetState(void)
{
	return currentState;
}

void SBanana::SetState(SBanana::CURRENT_STATE state)
{
	currentState = state;
}
