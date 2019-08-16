#include "Rect2D.h"

Rect2D::Rect2D()
	: min(0,0,0)
	, max(0,0,0)
{
}

Rect2D::Rect2D(Vector3 min, Vector3 max)
	: min(min)
	, max(max)
{
}

Rect2D::~Rect2D()
{
}

bool Rect2D::isWithin(Vector3 pos, Vector3* penetration)
{

	bool ret = (pos.x >= min.x && pos.x <= max.x && pos.y <= min.y && pos.y >= max.y);

	Vector3 temp1, temp2;
	temp1 = pos - min;
	temp2 = max - pos;

	if (!penetration)
	{
		penetration->x = temp1.x < temp2.x ? temp1.x : temp2.x;
		penetration->y = temp1.y < temp2.y ? temp1.y : temp2.y;
	}

	return ret;
}
