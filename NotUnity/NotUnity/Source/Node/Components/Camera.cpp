#include "Camera.h"

#include "Axis.h"
#include "Transform.h"
#include "../../Utility/Input/ControllerKeyboard.h"

Camera::Camera(std::string name) 
	: Node(name)
	, speed(5)
	, mode(DEBUG)
	, t(nullptr)
{
	axis = AddChild<Axis>();
	view.SetToIdentity();
}

Camera::~Camera()
{
	t = nullptr;
	axis = nullptr;
}

void Camera::Start()
{	
	t = parent->GetChild<Transform>();
	
	Node::Start();
}

void Camera::Update(double dt)
{
	float dtf = static_cast<float>(dt);
	ControllerKeyboard* kb = ControllerKeyboard::Instance();
	switch (mode)
	{
	case DEBUG:
		if (kb->IsKeyDown('W'))
			t->translate += axis->view * dtf * speed;
		if (kb->IsKeyDown('S'))
			t->translate -= axis->view * dtf * speed;
		if (kb->IsKeyDown('A'))
			t->translate -= axis->RightNoY() * dtf * speed;
		if (kb->IsKeyDown('D'))
			t->translate += axis->RightNoY() * dtf * speed;
		if (kb->IsKeyDown('Q'))
			t->translate -= axis->DefaultUp() * dtf * speed;
		if (kb->IsKeyDown('E'))
			t->translate += axis->DefaultUp() * dtf * speed;
		break;
	}

	Vector3 target = t->translate + axis->view;
	view.SetToLookAt(t->translate.x, t->translate.y, t->translate.z, target.x, target.y, target.z, axis->up.x, axis->up.y, axis->up.z);
	Node::Update(dt);
}

void Camera::End()
{
	Node::End();
}

Camera * Camera::SetMode(MODE m)
{
	mode = m;
	return this;
}

Camera * Camera::SetSpeed(float s)
{
	speed = s;
	return this;
}

Mtx44* Camera::GetViewMtx()
{
	return &view;
}
