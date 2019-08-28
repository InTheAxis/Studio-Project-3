#include "SKiwi.h"
#include "../../NotUnity/Source/Node/Components/KinematicBody.h"
#include "../../NotUnity/Source/Node/Scripts/Projectile.h"

SKiwi::SKiwi() 
	: currentState(IDLE)
	, shouldAttack(false)
	, inteval(0.f)
	, atkIn(0.f)
	, randSpeed(Math::RandFloatMinMax(2.f, 4.f))
	, speed(0)
	, boss(false)
{
}

SKiwi::~SKiwi()
{
}

void SKiwi::Update(Vector3& dest, Vector3& enemyPos, KinematicBody* kb, double dt)
{
	atkIn += 1.f * static_cast<float>(dt);
	shouldAttack = false;

	if (atkIn >= randSpeed)
	{
		if ((dest - enemyPos).LengthSquared() > 1.f) //2) Will headbutt player after awile
			currentState = ATTACK;

		if ((dest - enemyPos).LengthSquared() <= 0.5f)//3) Prevent AI from crossing player
			currentState = WAIT;
	}
	else
	{
		if ((dest - enemyPos).LengthSquared() < 3.f) // 4) Will move away from player after headbutt it
			currentState = REPEL;
		else
		{
			if ((dest - enemyPos).LengthSquared() > 3.5f) //1) Will move to player when spawn from far
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

void SKiwi::Attack(Projectile* p, Vector3& enemyPos, Vector3& direction, double dt)
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

bool SKiwi::SelfInflict()
{
	return false;
}

bool SKiwi::HasArmor()
{
	return true;
}

void SKiwi::Boss(bool boss)
{
	this->boss = boss;
}

SKiwi::CURRENT_STATE SKiwi::GetState(void)
{
	return currentState;
}

void SKiwi::SetState(SKiwi::CURRENT_STATE state)
{
	currentState = state;
}
