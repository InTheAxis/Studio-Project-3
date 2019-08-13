#ifndef CAMERA_BASE_H
#define CAMERA_BASE_H

#include "Vector3.h"

class CameraBase
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	float camSpeed;

	CameraBase();
	~CameraBase();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
};

#endif