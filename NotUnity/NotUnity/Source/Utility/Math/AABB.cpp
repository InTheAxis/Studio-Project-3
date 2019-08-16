#include "AABB.h"

AABB::AABB()
	: origin(nullptr)
	, type(COL_TYPE::CIRCLE)
	, radius(0)	
{
}

AABB::~AABB()
{
	origin = nullptr;
}

AABB * AABB::SetMode(COL_TYPE type)
{
	this->type = type;
	return this;
}

AABB * AABB::SetOrigin(Vector3 * pos)
{
	if (!pos)
		Debug::LogWarning("Position is null");
	origin = pos;
	return this;
}

AABB * AABB::SetBoundary(Vector3 min, Vector3 max)
{
	if (min.x > max.x || min.x > max.x)
		Debug::LogWarning("Min boundary is bigger than Max boundary!");	

	rect.max = max;
	rect.min = min;
	return this;
}

AABB * AABB::SetRadius(float radius)
{
	this->radius = radius;
	return this;
}

bool AABB::IsInside(Vector3 pos, Vector3* penetration)
{
	bool ret = false;
	switch (type)
	{
	case COL_TYPE::CIRCLE:		
		ret = ((pos - *origin).LengthSquared() < radius * radius);
		if (ret) *penetration = (pos - *origin).Normalized() * (radius - (pos - *origin).Length());
		return ret;
		break;
	case COL_TYPE::RECT2D:
		return rect.isWithin(pos, penetration);
		break;
	}
	Debug::LogWarning("Invalid COllider type");
	return ret;
}
