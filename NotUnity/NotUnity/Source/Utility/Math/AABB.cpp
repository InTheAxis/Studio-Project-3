#include "AABB.h"

AABB::AABB()
	: origin(0,0,0)
	, type(COL_TYPE::CIRCLE)
	, radius(0)	
{
}

AABB::~AABB()
{
}

AABB * AABB::SetType(COL_TYPE type)
{
	this->type = type;
	return this;
}

AABB * AABB::SetOrigin(Vector3 pos)
{
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
		{
			float len = (pos - origin).Length();
			ret = (len < radius);
			if (ret) *penetration = (pos - origin).Normalized() * (radius - len);
			return ret;
			break;
		}
		case COL_TYPE::RECT2D:
		{
			return rect.IsWithin(pos, penetration);
			break;
		}
	}
	Debug::LogWarning("Invalid COllider type");
	return ret;
}

bool AABB::IsOverlap(AABB other, Vector3 * penetration)
{
	switch (type)
	{
	case COL_TYPE::CIRCLE:
		if (other.type == COL_TYPE::RECT2D)
			return InternalOverlap(rect, other, penetration);
		else if (other.type == COL_TYPE::CIRCLE)
			return InternalOverlap(other, penetration);
		break;
	case COL_TYPE::RECT2D:
		if (other.type == COL_TYPE::RECT2D)
			return InternalOverlap(rect, other.rect, penetration);
		else if (other.type == COL_TYPE::CIRCLE)
			return InternalOverlap(rect, other, penetration);
		break;
	}
	return false;
}

Rect2D AABB::GetRect() const
{
	return rect;
}

COL_TYPE AABB::GetType() const
{
	return type;
}

float AABB::GetRadius() const
{
	return radius;
}

bool AABB::InternalOverlap(Rect2D rect, Rect2D other, Vector3 * penetration)
{
	return rect.IsOverlap(other, penetration);
}

bool AABB::InternalOverlap(Rect2D rect, AABB circle, Vector3 * penetration)
{
	return false;
}

bool AABB::InternalOverlap(AABB circle, Vector3 * penetration)
{
	Vector3 relDisp = origin - circle.origin;
	float len = relDisp.Length();
	if (len < radius + circle.radius)
	{
		*penetration = relDisp.Normalized() * (radius + circle.radius - len);
		return true;
	}
	return false;
}
