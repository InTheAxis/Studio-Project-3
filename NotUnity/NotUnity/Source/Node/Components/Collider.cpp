#include "Collider.h"

#include "../Components/Transform.h"
#include "../Components/ColliderRender.h"
#include "../Manager/MgrCollision.h"

Collider::Collider(std::string name)
	: Node(name)
	, isTrigger(false)
	, collided(false)
	, triggered(false)
	, tag("coll")
{
	MgrCollision::Instance()->RegisterColl(this);
}

Collider::~Collider()
{
}

void Collider::Start()
{
	t = gameObject->GetTransform();
	//cr = AddChild<ColliderRender>();
	//cr->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("default"))->SetGameObj(gameObject);
	//std::vector<Vector3> temp{ aabb.GetRect().max, aabb.GetRect().min };
	//cr->AttachPoints(temp);
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
		info.coll = this;
		info.other = coll;
		info.penetration = pene;
		if (isTrigger)
		{
			OnTriggerStay(info);
			if (!triggered)
			{
				triggered = true;
				OnTriggerEnter(info);
			}
		}
		else
		{
			OnCollideStay(info);
			if (!collided)
			{
				collided = true;
				OnCollideEnter(info);				
			}
		}

		info.coll = coll;
		info.other = this;
		info.penetration = -pene;
		if (coll->isTrigger)
		{
			coll->OnTriggerStay(info);
			if (!coll->triggered)
			{
				coll->triggered = true;
				coll->OnTriggerEnter(info);				
			}
		}
		else
		{
			coll->OnCollideStay(info);
			if (!coll->collided)
			{
				coll->collided = true;
				coll->OnCollideEnter(info);
			}
		}
	}
	else
	{
		info.coll = this;
		info.other = coll;
		info.penetration = pene;
		if (triggered)
			OnTriggerExit(info);
		if (collided)
			OnCollideExit(info);

		info.coll = coll;
		info.other = this;
		info.penetration = -pene;
		if (coll->collided)
			coll->OnCollideExit(info);
		if (coll->triggered)
			coll->OnTriggerExit(info);

		collided = false;
		triggered = false;
		coll->collided = false;
		coll->triggered = false;
	}
}