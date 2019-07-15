#ifndef CAMERA_BASE_H
#define CAMERA_BASE_H

#include "Mtx44.h"

class CameraBase
{
public:
	CameraBase();
	virtual ~CameraBase();

	void Init(const Vector3& pos, const Vector3& target);
	virtual void Update(const double dt) = 0;
	void Reset();

	void RotateView(const double dt, const float yaw, const float pitch, const float speedMultiplier = 1.0f);
	void UpdateTargetRightUp();

	//getter setters

	Mtx44 GetViewMatrix() const;
	Vector3 GetCameraPos() const;
	Vector3 GetCameraTarget() const;
	Vector3 GetCameraUp() const;

private:
	Vector3 m_defaultPosition, m_defaultTarget, m_defaultUp;
	Vector3 m_position, m_target, m_up;
	Vector3 m_view, m_right;	
};

#endif // CAMERA_BASE_H