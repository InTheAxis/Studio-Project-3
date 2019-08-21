#include "Strategy.h"

Strategy::Strategy(): dest(0.f,0.f,0.f)
{
}

Strategy::~Strategy()
{
}

int Strategy::CalDest(Vector3 dest, Vector3 enemyPos)
{
	return ((dest.x - enemyPos.x) * (dest.x - enemyPos.x) + (dest.y - enemyPos.y) * (dest.y - enemyPos.y));
}

void Strategy::Attack()
{
}

bool Strategy::Update(Vector3 & dest, Vector3 & enemyPos, double dt)
{
	return true;
}