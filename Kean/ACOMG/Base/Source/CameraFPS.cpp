#include "CameraFPS.h"
#include "Application.h"
#include "Mtx44.h"

CameraFPS::CameraFPS()
{
}

CameraFPS::~CameraFPS()
{
}

void CameraFPS::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	camSpeed = 150;
}

void CameraFPS::Update(double dt)
{
	if (Application::IsKeyPressed('A'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position -= right * camSpeed * (float)dt;
		target -= right * camSpeed * (float)dt;
	}
	if (Application::IsKeyPressed('D'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position += right * camSpeed * (float)dt;
		target += right * camSpeed * (float)dt;
	}
	if (Application::IsKeyPressed('W'))
	{
		Vector3 view = (target - position).Normalized();
		view.y = 0;
		position += view * camSpeed * (float)dt;
		target += view * camSpeed * (float)dt;
	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 view = (target - position).Normalized();
		view.y = 0;
		position -= view * camSpeed * (float)dt;
		target -= view * camSpeed * (float)dt;
	}
	if (Application::IsKeyPressed(VK_SHIFT))
	{
		position.y -= camSpeed * (float)dt;
		target.y -= camSpeed * (float)dt;
	}
	if (Application::IsKeyPressed(VK_SPACE))
	{
		position.y += camSpeed * (float)dt;
		target.y += camSpeed * (float)dt;
	}
	if (Application::IsKeyPressed(VK_LEFT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(camSpeed * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(-camSpeed * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	if (Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(camSpeed * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(-camSpeed * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	//Update the camera direction based on mouse move
	// left-right rotate
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(-camSpeed * Application::camera_yaw * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	{
		float pitch = (float)(-camSpeed * Application::camera_pitch * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void CameraFPS::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}