#include "StrategyOne.h"

StrategyOne::StrategyOne(): currentState(IDLE), shouldAttack(false)
{
}

StrategyOne::~StrategyOne()
{
}

void StrategyOne::Update(Vector3& dest, Vector3& enemyPos, double dt)
{
	int distanceHeroToEnemy = CalDest(dest, enemyPos);

	if (distanceHeroToEnemy < STATE_ATTACK * STATE_ATTACK)
	{		
		if (distanceHeroToEnemy < STATE_REPEL * STATE_REPEL)
			currentState = REPEL;
		else
			currentState = ATTACK;
	}
	else
		currentState = IDLE;

	switch(currentState)
	{
	case ATTACK:
		shouldAttack = true;
		break;
	case REPEL:
		break;
	default: //IDLE
		break;
	}
}

bool StrategyOne::Attack()
{
	return shouldAttack;
}

void StrategyOne::SetDest(float x, float y)
{
	dest.x = x;
	dest.y = y;
}

float StrategyOne::GetDestX(void)
{
	return dest.x;
}

float StrategyOne::GetDestY(void)
{
	return dest.y;
}

Vector3 StrategyOne::GetDest(void)
{
	return dest;
}

StrategyOne::CURRENT_STATE StrategyOne::GetState(void)
{
	return currentState;
}

void StrategyOne::SetState(StrategyOne::CURRENT_STATE state)
{
	currentState = state;
}
