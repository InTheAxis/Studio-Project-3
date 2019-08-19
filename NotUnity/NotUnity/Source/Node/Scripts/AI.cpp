#include "AI.h"
#include "../Manager/MgrGraphics.h"
#include "../../Node/GameObj.h"

AI::AI(std::string name) : Node(name), EnemyNames(""), playerTrans(0.f, 0.f, 0.f), health(0.f), atkSpeed(0.f), damage(0.f)
{
}

AI::~AI()
{
}

void AI::Start()
{
	gameObject->AddScript<Strategy>();
	Node::Start();
}

void AI::Update(double dt)
{
	if (gameObject->IsActive())
	{
		gameObject->GetScript<Strategy>()->SetPlayerTrans(playerTrans);
		gameObject->GetScript<Strategy>()->SetEnemyName(gameObject->GetName());
	}
	Node::Update(dt);
}

//void AI::Move()
//{
//	
//}

void AI::SetHealth(int health)
{
	this->health = health;
}

float AI::GetHealth()
{
	return health;
}

void AI::SetAtkSpeed(int atkSpeed)
{
	this->atkSpeed = atkSpeed;
}

float AI::GetAtkSpeed()
{
	return atkSpeed;
}

void AI::SetDamage(int damage)
{
	this->damage = damage;
}

float AI::GetDamage()
{
	return damage;
}

void AI::SetPlayerTrans(Vector3 trans)
{
	playerTrans = trans;
}

void AI::End()
{
	Node::End();
}