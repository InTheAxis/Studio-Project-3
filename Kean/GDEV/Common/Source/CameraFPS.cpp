#include "CameraFPS.h"
#include "MouseController.h"
#include "KeyboardController.h"

CameraFPS::CameraFPS()
{
}

CameraFPS::~CameraFPS()
{
}

void CameraFPS::Update(double dt)
{
	//get mouse delta
	Vector3 mouseDelta;
	MouseController::GetInstance()->GetMouseDelta(mouseDelta.x, mouseDelta.y);
	mouseDelta.x = Math::DegreeToRadian(mouseDelta.x);
	mouseDelta.y = Math::DegreeToRadian(mouseDelta.y);

	//rotate view dir
	RotateView(dt, mouseDelta.x, mouseDelta.y, CAM_SPEED);
	//update target, right and up, normalized
	UpdateTargetRightUp();
}