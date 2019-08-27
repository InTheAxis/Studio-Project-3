#include "STomato.h"
#include "../../NotUnity/Source/Node/Components/KinematicBody.h"
#include "../../NotUnity/Source/Node/Scripts/Projectile.h"

STomato::STomato()
	: currentState(IDLE)
	, shouldAttack(false)
	, inteval(0.f)
	, speed(0)
	, boss(false)
{
}

STomato::~STomato()
{
}

void STomato::Update(Vector3& dest, Vector3& enemyPos, KinematicBody* kb, double dt)
{
	shouldAttack = true;

	if ((dest - enemyPos).LengthSquared() > 4.f)
		currentState = ATTACK;
	else
		currentState = IDLE;

	switch(currentState)
	{
	case ATTACK:
		kb->ApplyForce((dest - enemyPos).Normalized());
		break;
	default: //IDLE
		kb->ResetVel(1, 0);
		break;
	}
}

void STomato::Attack(Projectile* p, Vector3& enemyPos, Vector3& direction, double dt)
{
	inteval += 1.f * static_cast<float>(dt);

	if (boss)
		speed = 2.5f;
	else
		speed = 3.5f;

	if (shouldAttack && inteval >= speed)
	{
		if (p)
		{
			if (direction.LengthSquared() == 0)
				p->Discharge(enemyPos, direction * 10);
			else
				p->Discharge(enemyPos, direction.Normalized() * 10);
			p->GetGameObj()->ActiveSelf(true);
			Debug::Log(direction.Normalized());
		}
		inteval = 0;
	}
}

bool STomato::SelfInflict()
{
	return false;
}

bool STomato::HasArmor()
{
	return false;
}

void STomato::Boss(bool boss)
{
	this->boss = boss;
}

STomato::CURRENT_STATE STomato::GetState(void)
{
	return currentState;
}

void STomato::SetState(STomato::CURRENT_STATE state)
{
	currentState = state;
}
