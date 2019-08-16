#ifndef AABB_H
#define AABB_H

#include "Rect2D.h"
#include "../WinDebug.h"

enum class COL_TYPE
{
	CIRCLE,
	RECT2D,
};

class AABB 
{
public:
	AABB();
	~AABB();

	AABB* SetType(COL_TYPE type);
	AABB* SetOrigin(Vector3 pos);
	AABB* SetBoundary(Vector3 min, Vector3 max);
	AABB* SetRadius(float radius);
	bool IsInside(Vector3 pos, Vector3* penetration = nullptr);
	bool IsOverlap(AABB other, Vector3* penetration = nullptr);

private:
	Vector3 origin;

	COL_TYPE type;
	
	float radius;
	Rect2D rect;

	bool InternalOverlap(Rect2D rect, Rect2D other, Vector3* penetration = nullptr);
	bool InternalOverlap(Rect2D rect, AABB circle, Vector3* penetration = nullptr);
	bool InternalOverlap(AABB circle, Vector3* penetration = nullptr);	
};

#endif
