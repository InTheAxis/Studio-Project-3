#include "Projectile.h"
#include "../Manager/MgrGameObj.h"
#include "../Manager/MgrGraphics.h"
#include "../../Node/GameObj.h"
#include "../../Node/Components/Transform.h"
#include "../../Node/Components/KinemeticBody.h"
#include "../../Node/Components/Sprite.h"
#include "../../Node/Components/Collider.h"

void OnHit(ColInfo info)
{
	info.coll->GetGameObj()->ActiveSelf(false);
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
	coll->OnCollide += OnHit;
}

void Projectile::OnDisable()
{
	if (coll)
		coll->OnCollide -= OnHit;	
	if (kinb)
		kinb->ResetVel(1, 1);
}

void Projectile::Start()
{		
	t = gameObject->GetTransform();	
	float size = 0.1f;
	t->scale.Set(size, size, size);

	kinb = AddChild<KinemeticBody>();
	kinb->SetGameObj(gameObject);
	kinb->useGravity = false;
	kinb->maxVel.Set(10, 10, 0);

	sprite = AddChild<Sprite>();
	sprite->SetGameObj(gameObject);
	sprite->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("quad"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("default"));

	coll = AddChild<Collider>();
	coll->SetGameObj(gameObject);
	coll->CreateAABB(Vector3(-size * 0.5f, -size * 0.5f), Vector3(size * 0.5f, size * 0.5f));
	Node::Start();
}

void Projectile::Update(double dt)
{
	if (m_lifetime > 5)
		gameObject->ActiveSelf(false);
	kinb->UpdateSuvat(dt);
	Debug::Log(kinb->GetVel());
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