#include "Projectile.h"
#include "../Manager/MgrGameObj.h"
#include "../Manager/MgrGraphics.h"
#include "../../Node/GameObj.h"
#include "../../Node/Components/Transform.h"
#include "../../Node/Components/KinemeticBody.h"

Projectile::Projectile(std::string name)
	: Node(name)
{
}

Projectile::~Projectile()
{
}

void Projectile::Start()
{		
	t = gameObject->GetTransform();	
	kinb = AddChild<KinemeticBody>();
	kinb->SetGameObj(gameObject);
	Node::Start();
}

void Projectile::Update(double dt)
{
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
