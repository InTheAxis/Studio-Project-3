#include "SCarrot.h"
#include "../../NotUnity/Source/Node/Components/KinematicBody.h"
#include "../../NotUnity/Source/Node/Scripts/Projectile.h"

SCarrot::SCarrot() 
	: currentState(IDLE)
	, shouldAttack(false)
	, inteval(0.f)
	, atkIn(0.f)
	, randSpeed(Math::RandFloatMinMax(0.5f, 1.5f))
	, speed(0)
	, boss(false)
{
}

SCarrot::~SCarrot()
{
}

void SCarrot::Update(Vector3& dest, Vector3& enemyPos, KinematicBody* kb, double dt)
{
	atkIn += 1.f * static_cast<float>(dt);
	shouldAttack = false;

	if (atkIn >= randSpeed)
	{
		if ((dest - enemyPos).Length() > 1.f) //2) Will headbutt player after awile
			currentState = ATTACK;

		if ((dest - enemyPos).Length() <= 0.5f)//3) Prevent AI from crossing player
			currentState = WAIT;
	}
	else
	{
		if ((dest - enemyPos).Length() < 3.f) // 4) Will move away from player after headbutt it
			currentState = REPEL;
		else
		{
			if ((dest - enemyPos).Length() > 3.5f) //1) Will move to player when spawn from far 0.8f
				currentState = ATTACK;
			else
				currentState = IDLE;
		}
	}

	switch (currentState)
	{
	case ATTACK:
		kb->ApplyForce((dest - enemyPos).Normalized());
		break;
	case WAIT:
		kb->ResetVel(1, 0);
		atkIn = 0.f;
		break;
	case REPEL:
		kb->ApplyForce(-(dest - enemyPos).Normalized());
		break;
	default: //IDLE
		kb->ResetVel(1, 0);
		break;
	}
}

void SCarrot::Attack(Projectile* p, Vector3& enemyPos, Vector3& direction, double dt)
{
	inteval += 1.f * static_cast<float>(dt);

	if (boss)
		speed = 2.f;
	else
		speed = 3.f;

	if (shouldAttack && inteval >= speed)
	{
		if (p)
		{
			if (direction.Length() == 0)
				p->Discharge(enemyPos, direction * 10);
			else
				p->Discharge(enemyPos, direction.Normalized() * 10);
			p->GetGameObj()->ActiveSelf(true);
		}
		inteval = 0.f;
	}
}

bool SCarrot::SelfInflict()
{
	return false;
}

bool SCarrot::HasArmor()
{
	return false;
}

void SCarrot::Boss(bool boss)
{
	this->boss = boss;
}

SCarrot::CURRENT_STATE SCarrot::GetState(void)
{
	return currentState;
}

void SCarrot::SetState(SCarrot::CURRENT_STATE state)
{
	currentState = state;
}
