#include "CameraBase.h"

CameraBase::CameraBase()
{
}

CameraBase::~CameraBase()
{
}

void CameraBase::Init(const Vector3& pos, const Vector3& target)
{
	this->m_position = m_defaultPosition = pos;
	this->m_target = m_defaultTarget = target;
	this->m_up = m_defaultUp = Vector3(0, 1, 0);
	this->m_view = (target - m_position).Normalized();
	this->m_right = m_view.Cross(m_up).Normalized();
}

void CameraBase::Reset()
{
	m_position = m_defaultPosition;
	m_target = m_defaultTarget;
	m_up = m_defaultUp;
}

void CameraBase::RotateView(const double dt, const float yaw, const float pitch, const float speedMultiplier)
{
	Mtx44 rotation;
	float pitchCopy = pitch;
	//prevent y-overflow
	if (m_view.y > 0.8f && pitchCopy < 0 || m_view.y < -0.8f && pitchCopy > 0)
		pitchCopy = 0;
	//pitch
	rotation.SetToRotation(-pitchCopy * speedMultiplier * (float)dt, m_right.x, m_right.y, m_right.z);
	m_view = rotation * m_view;
	//yaw
	rotation.SetToRotation(-yaw * speedMultiplier * (float)dt, m_up.x, m_up.y, m_up.z);
	m_view = rotation * m_view;

	m_view.Normalize();
}

void CameraBase::UpdateTargetRightUp()
{
	m_target = m_position + m_view;
	m_right = (m_view.Cross(m_defaultUp)).Normalized();
	m_up = (m_right.Cross(m_view)).Normalized();
}


Mtx44 CameraBase::GetViewMatrix() const
{
	Mtx44 result;
	result.SetToLookAt(m_position.x, m_position.y, m_position.z,
		m_target.x, m_target.y, m_target.z,
		m_up.x, m_up.y, m_up.z);
	return result;
}
Vector3 CameraBase::GetCameraPos() const
{
	return m_position;
}
Vector3 CameraBase::GetCameraTarget() const
{
	return m_target;
}
Vector3 CameraBase::GetCameraUp() const
{
	return m_up;
}