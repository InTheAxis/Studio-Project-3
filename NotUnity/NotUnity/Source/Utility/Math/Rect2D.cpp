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

bool Rect2D::IsWithin(Vector3 pos, Vector3* penetration)
{

	Vector3 minimum, maximum;
	minimum = min + center;
	maximum = max + center;
	bool ret = (pos.x >= minimum.x && pos.x <= maximum.x && pos.y <= minimum.y && pos.y >= maximum.y);

	Vector3 temp1, temp2;
	temp1 = pos - minimum;
	temp2 = maximum - pos;

	if (!penetration)
	{
		penetration->x = temp1.x < temp2.x ? temp1.x : temp2.x;
		penetration->y = temp1.y < temp2.y ? temp1.y : temp2.y;
	}

	return ret;
}

bool Rect2D::IsOverlap(Rect2D other, Vector3* penetration)
{	
	float points[8] =
	{
		center.x + min.x, 
		center.x + max.x, 
		center.y + min.y, 
		center.y + max.y, 
		other.center.x + other.max.x,
		other.center.x + other.min.x, 
		other.center.y + other.max.y,
		other.center.y + other.min.y,
	};
		
	bool ret = (points[0] < points[4] &&
		points[1] > points[5] &&
		points[2] < points[6] &&
		points[3] > points[7]);
	if (ret)
	{
		penetration->x = (points[0] - points[5]);
		penetration->y = (points[2] - points[7]);
	}

	return ret;
}
