#include "AI.h"
#include "../Manager/MgrGraphics.h"
#include "../../Node/GameObj.h"
#include"../Scripts/Spawner.h"

AI::AI(std::string name) : Node(name), playerTrans(0.f, 0.f, 0.f), health(0.f), 
damage(0.f),strategy(nullptr), direction(0.f,0.f,0.f), worldHeight(0.f), dead(false)
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
	
	Node::Start();
}

void AI::Update(double dt)
{
	direction = playerTrans - gameObject->GetTransform()->translate;

	if (strategy == NULL)
		ChangeStrategy(new StrategyOne(), false);

	if (strategy != NULL)
	{
		strategy->SetDest(playerTrans.x, playerTrans.y);
		strategy->Update(playerTrans, gameObject->GetTransform()->translate, dt);
	}

	if (direction.LengthSquared() > 1)
	{
		kineB->ApplyForce(direction);

		if (direction.x * kineB->GetVel().x < 0.f || Math::FIsEqual(kineB->GetVel().x, 0.f))
			kineB->ResetVel(1, 0);
		else
			kineB->ApplyForce(Vector3(-direction.x * kineB->mass * kineB->frictionCoeff, 0, 0));
	}
	else
	{
		//if (kineB->GetVel().x < 0)
		//	kineB->ResetVel(1, 0);
		//else if (Math::FIsEqual(kineB->GetVel().x, 0))
		//	kineB->ResetVel(1, 0);
		//else
		//	kineB->ApplyForce(kineB->frictionCoeff * kineB->mass * 0.1f);

		//if (direction.x * kineB->GetVel().x < 0.f || Math::FIsEqual(kineB->GetVel().x, 0.f))
		//	kineB->ResetVel(1, 0);
		//else
		//	kineB->ApplyForce(Vector3(-direction.x * kineB->mass * kineB->frictionCoeff, 0, 0));
		
		health -= 1;
	}

	if (gameObject->GetTransform()->translate.y > worldHeight)
	{
		kineB->useGravity = true;
	}
	else
	{
		gameObject->GetTransform()->translate.y = worldHeight;
		kineB->useGravity = false;
		kineB->ResetVel(0, 1);
	}

	//if (direction.LengthSquared() <= 0)
	//	Debug::Log("die");

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

void AI::SetPlayerTrans(Vector3 trans)
{
	playerTrans = trans;
}

void AI::End()
{
	Node::End();
}