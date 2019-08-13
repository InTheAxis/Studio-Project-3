#ifndef CAMERA_FPS_H
#define CAMERA_FPS_H

#include "CameraBase.h"

class CameraFPS : public CameraBase
{
public:
	CameraFPS();
	virtual ~CameraFPS();

	virtual void Update(const double dt);

private:
	const float CAM_SPEED = 100.f;
};

#endif