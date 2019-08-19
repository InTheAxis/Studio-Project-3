#include "Collider.h"

#include "../Components/Transform.h"
#include "../Components/Transform.h"
#include "../Manager/MgrCollision.h"

Collider::Collider(std::string name) 
	: Node(name)
	, isTrigger(false)
{
	MgrCollision::Instance()->RegisterColl(this);
}

Collider::~Collider()
{
}

void Collider::Start()
{
	t = gameObject->GetTransform();
	Node::Start();
}

void Collider::Update(double dt)
{
	aabb.SetOrigin(t->translate);
	Node::Update(dt);
}

void Collider::End()
{
	Node::End();
}

Collider * Collider::CreateAABB(float radius)
{	
	aabb.SetRadius(radius);
	aabb.SetType(COL_TYPE::CIRCLE);
	return this;
}

Collider * Collider::CreateAABB(Vector3 min, Vector3 max)
{
	aabb.SetBoundary(min, max);
	aabb.SetType(COL_TYPE::RECT2D);
	return this;
}

void Collider::CheckCollision(Collider * coll)
{
	Vector3 pene;
	ColInfo info;
	if (aabb.IsOverlap(coll->aabb, &pene))
	{
		info.other = coll;
		info.penetration = pene;
		if (isTrigger)
			OnTrigger(info);
		else
			OnCollide(info);

		info.other = this;
		info.penetration = -pene;
		if (coll->isTrigger)
			coll->OnTrigger(info);
		else
			coll->OnCollide(info);
	}
}