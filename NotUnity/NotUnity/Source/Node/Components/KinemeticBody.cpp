#include "KinemeticBody.h"
#include "Transform.h"

KinemeticBody::KinemeticBody(std::string name) : Node(name)
{
	acceleration.SetZero();
	force.SetZero();
	velocity.SetZero();
	gravity.Set(0, -1, 0);
	
	mass = 1;
	maxVel.Set(1, 1, 0);
	frictionCoeff = 1;
	useGravity = true;
	gravScale = 1;
}

KinemeticBody::~KinemeticBody()
{
}

void KinemeticBody::Start()
{
	Node::Start();
}

void KinemeticBody::Update(double dt)
{
	UpdateSuvat(dt);

	Node::Update(dt);
}

void KinemeticBody::End()
{
	Node::End();
}

void KinemeticBody::ApplyForce(Vector3 force)
{
	this->force += force;
}

void KinemeticBody::ApplyImpulse(Vector3 impulse)
{
	this->velocity = impulse;
}

void KinemeticBody::UpdateSuvat(double dt)
{
	float dtf = static_cast<float>(dt);
	acceleration = force * (1 / mass);
	velocity += acceleration * dtf;
	if (useGravity)
	{
		velocity += gravScale * gravity * dtf;
	}
	
	velocity.x = Math::Clamp(velocity.x, -maxVel.x, maxVel.x);
	velocity.y = Math::Clamp(velocity.y, -maxVel.y, maxVel.y);
	velocity.z = Math::Clamp(velocity.z, -maxVel.x, maxVel.z);

	gameObject->GetTransform()->translate += velocity * dtf;
}

void KinemeticBody::ResetForce()
{
	force.SetZero();
}

void KinemeticBody::ResetVel(bool x, bool y, bool z)
{
	if (x) velocity.x = 0;
	if (y) velocity.y = 0;
	if (z) velocity.z = 0;
}

Vector3 KinemeticBody::GetVel() const
{
	return velocity;
}


