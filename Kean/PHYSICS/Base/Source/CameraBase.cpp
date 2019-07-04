#include "CameraBase.h"
#include "Application.h"
#include "Mtx44.h"

CameraBase::CameraBase()
{
	Reset();
}

CameraBase::~CameraBase()
{
}

void CameraBase::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

void CameraBase::Reset()
{
	position.Set(0, 0, 1);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void CameraBase::Update(double dt)
{
	static const float camSpeed= 20.f;
	if(Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed('A'))
	{
		position.x -= (float)(camSpeed * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_RIGHT) || Application::IsKeyPressed('D'))
	{
		position.x += (float)(camSpeed * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed('W'))
	{
		position.y += (float)(camSpeed * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed('S'))
	{
		position.y -= (float)(camSpeed * 0.2 * dt);
	}
}