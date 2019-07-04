#ifndef CAMERA_FPS_H
#define CAMERA_FPS_H

#include "CameraBase.h"

class CameraFPS : public CameraBase
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	double camYaw, camPitch;

	CameraFPS();
	~CameraFPS();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Update(double dt);
	void Reset();
};

#endif