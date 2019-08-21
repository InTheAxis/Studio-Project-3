#ifndef STRATEGY_H
#define STRATEGY_H

#include <vector>
#include "../Math/Vector3.h"
#include "../../NotUnity/Source/Node/Components/KinemeticBody.h"

class Strategy
{
public:
	Strategy();
	~Strategy();

	virtual int CalDest(Vector3 dest, Vector3 enemyPos);
	virtual void Attack();
	virtual void Update(Vector3& dest, Vector3& enemyPos, double dt);

	virtual void SetDest(float x, float y) = 0;
	virtual float GetDestX() = 0;
	virtual float GetDestY() = 0;
	virtual Vector3 GetDest() = 0;
	
protected:
	Vector3 dest;
};
#endif