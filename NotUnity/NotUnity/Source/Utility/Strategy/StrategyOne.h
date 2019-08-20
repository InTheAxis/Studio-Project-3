#ifndef STRATEGYONE_H
#define STRATEGYONE_H

#include <vector>
#include "Strategy.h"
#include "../Math/Vector3.h"

class StrategyOne : public Strategy
{
public:
	StrategyOne();
	~StrategyOne();

	void Update(Vector3& dest, Vector3& enemyPos, double dt);
	void Attack();

	void SetDest(float x, float y);
	float GetDestX();
	float GetDestY();
	Vector3 GetDest();

	enum CURRENT_STATE
	{
		IDLE,
		REPEL,
		ATTACK,
	};

	enum STATE_RANGE
	{
		STATE_ATTACK = 25,
		STATE_REPEL = 2
	};

	StrategyOne::CURRENT_STATE GetState();
	void SetState(StrategyOne::CURRENT_STATE state);

private:
	StrategyOne::CURRENT_STATE currentState;
};
#endif