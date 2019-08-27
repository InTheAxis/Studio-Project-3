#include "Projectile.h"
#include "../Manager/MgrGameObj.h"
#include "../Manager/MgrGraphics.h"
#include "../../Node/GameObj.h"
#include "../../Node/Components/Transform.h"
#include "../../Node/Components/KinematicBody.h"
#include "../../Node/Components/Sprite.h"
#include "../../Node/Components/Collider.h"
#include "../../Node/Scripts/PlayerController.h"

void Projectile::OnHit(ColInfo info)
{
	if (info.other->tag == target)
		ActiveSelf(false);
}


Projectile::Projectile(std::string name)
	: Node(name)
{
}

Projectile::~Projectile()
{
}

void Projectile::OnEnable()
{
	coll->OnCollideEnter.Subscribe(&Projectile::OnHit, this, "hit");
}

void Projectile::OnDisable()
{
	if (coll)
		coll->OnCollideEnter.UnSubscribe("hit");
	if (kinb)
		kinb->ResetVel(1, 1);
}

void Projectile::Start()
{		
	t = gameObject->GetTransform();	
	float size = 0.1f;
	t->scale.Set(size, size, size);

	kinb = AddChild<KinematicBody>();
	kinb->SetGameObj(gameObject);
	kinb->useGravity = false;
	kinb->maxVel.Set(3, 3, 0);

	sprite = AddChild<Sprite>();
	sprite->SetGameObj(gameObject);
	sprite->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("quad"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("default"));

	coll = AddChild<Collider>();
	coll->SetGameObj(gameObject);
	coll->CreateAABB(size);
	coll->tag = "bullet";
	Node::Start();
}

void Projectile::Update(double dt)
{
	if (m_lifetime > 5)
		gameObject->ActiveSelf(false);
	coll->tag = "bullet" + target;
	kinb->UpdateSuvat(dt);
	Node::Update(dt);
}

void Projectile::End()
{
	Node::End();
}

void Projectile::Discharge(Vector3 origin, Vector3 vel)
{	
	t->translate = origin;
	kinb->ApplyImpulse(vel);
}

Projectile * Projectile::SetTarget(std::string target)
{
	this->target = target;	
	return this;
}
