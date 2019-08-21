#include "AI.h"
#include "../Manager/MgrGraphics.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../../Node/GameObj.h"
#include "../Scripts/Spawner.h"
#include "Projectile.h"

AI::AI(std::string name) 
	: Node(name)
	, playerTrans(0.f, 0.f, 0.f)
	, health(0.f)
	, damage(0.f)
	, strategy(nullptr)
	, direction(0.f,0.f,0.f)
	, dead(false)
	, enemyCount(0)
	, kineB(nullptr)
	, s(nullptr)
{
}

AI::~AI()
{
}

void AI::Start()
{
	kineB = AddChild<KinemeticBody>();
	kineB->SetGameObj(gameObject);
	kineB->maxVel.Set(2, 2, 0);
	kineB->frictionCoeff = 0.2f;
	kineB->gravScale = 5;
	kineB->useGravity = false;

	for (int i = 0; i < ammoCount; ++i)
	{
		projectile[i] = AddChild<GameObj>("bull" + std::to_string(i))->AddScript<Projectile>();
		projectile[i]->GetGameObj()->ActiveSelf(false);
	}

	if (strategy == NULL)
		ChangeStrategy(new StrategyOne(), false);
	
	Node::Start();
}

void AI::Update(double dt)
{ 
	direction = (playerTrans - gameObject->GetTransform()->translate).Normalized();

	strategy->SetDest(playerTrans.x, playerTrans.y);
	if (strategy->Update(playerTrans, gameObject->GetTransform()->translate, dt))
	{

	}
	if (ControllerKeyboard::Instance()->IsKeyPressed('7'))
	{
		Projectile* p = GetProjectile();
		if (p)
		{
			p->Discharge(gameObject->GetTransform()->translate, direction * 10);
			p->GetGameObj()->ActiveSelf(true);
		}
	}
	kineB->ApplyForce(direction);

	if (direction.x * kineB->GetVel().x < 0.f || Math::FIsEqual(kineB->GetVel().x, 0.f))
		kineB->ResetVel(1, 0);
	else
		kineB->ApplyForce(Vector3(-direction.x * kineB->mass * kineB->frictionCoeff, 0, 0));

	if (gameObject->GetTransform()->translate.y > GetWorldHeight() + 1)
	{
		kineB->useGravity = true;
	}
	else
	{
		gameObject->GetTransform()->translate.y = GetWorldHeight();
		kineB->useGravity = false;
		kineB->ResetVel(0, 1);
	}

	kineB->UpdateSuvat(dt);
	kineB->ResetForce();

	if (health < 0)
	{
		gameObject->ActiveSelf(false);
		health = 0;
		dead = true;
	}
	else
		dead = false;

	Node::Update(dt);
}

void AI::SetHealth(float health)
{
	this->health = health;
}

float AI::GetHealth()
{
	return health;
}

void AI::SetDamage(float damage)
{
	this->damage = damage;
}

float AI::GetDamage()
{
	return damage;
}

void AI::ChangeStrategy(Strategy* newStrategy, bool remove)
{
	remove = true;
	if (remove)
	{
		if (strategy != NULL)
		{
			delete strategy;
			strategy = NULL;
		}
	}

	strategy = newStrategy;
}

bool AI::IsDead()
{
	return dead;
}

AI* AI::SetTerrain(Spline * s)
{
	this->s = s;
	return this;
}

float AI::GetWorldHeight()
{
	//return gameObject->GetTransform()->translate.x;
	return s->Fn(gameObject->GetTransform()->translate.x);
}

Projectile * AI::GetProjectile()
{
	for (int i = 0; i < ammoCount; ++i)
	{
		if (!projectile[i]->IsActive())
  			return projectile[i];
	}
	return nullptr;
}

void AI::SetPlayerTrans(Vector3 trans)
{
	playerTrans = trans;
}

void AI::End()
{
	Node::End();
}