#include "Strategy.h"
#include "../Manager/MgrGraphics.h"
#include "../../Node/GameObj.h"
#include "../Components/KinemeticBody.h"

Strategy::Strategy(std::string name) : Node(name), playerTrans(0.f, 0.f, 0.f), currentState(IDLE), kineB(nullptr), enemyNames("")
{
}

Strategy::~Strategy()
{
}

void Strategy::Start()
{
	Node::Start();
}

void Strategy::Update(double dt)
{
	kineB = gameObject->AddComp<KinemeticBody>();
	kineB->maxVel.Set(2, 2, 0);
	kineB->frictionCoeff = 32;
	kineB->gravScale = 5;

	int disPlayerToEnemy = CalDis(playerTrans, gameObject->GetTransform()->translate);

	if (disPlayerToEnemy < STATE_ATTACK * STATE_ATTACK)
	{
		//if (disPlayerToEnemy < STATE_REPEL * STATE_REPEL)
		//	currentState = REPEL;
		if (disPlayerToEnemy < STATE_COLLIDE * STATE_COLLIDE)
			currentState = COLLIDE;
		else
			currentState = ATTACK;
	}
	else
		currentState = IDLE;

	switch (currentState)
	{
	case ATTACK:
		//gameObject->GetTransform()->translate.x = gameObject->GetTransform()->translate.x + (playerTrans.x - gameObject->GetTransform()->translate.x > 0 ? 1 : -1) * static_cast<float>(dt);
		if (playerTrans.x - gameObject->GetTransform()->translate.x > 5)
			kineB->ApplyForce(Vector3(1, 0, 0));
		if (playerTrans.x - gameObject->GetTransform()->translate.x < -5)
			kineB->ApplyForce(Vector3(-1, 0, 0));

		//Debug::Log("E Attack State");
		break;
	case REPEL:
		//Debug::Log("E Repel State");
		break;
	case COLLIDE:
		gameObject->GetScript<AI>()->SetHealth(gameObject->GetScript<AI>()->GetHealth() - 1);
		//Debug::Log(gameObject->GetScript<AI>()->GetHealth());
		break;
	default: //IDLE State
		if (kineB->GetVel().x < 0)
			kineB->ResetVel(1, 0);
		else if (Math::FIsEqual(kineB->GetVel().x, 0))
			kineB->ResetVel(1, 0);
		else
			kineB->ApplyForce(kineB->frictionCoeff * kineB->mass * 0.1f);
		break;
	}

	if (gameObject->GetTransform()->translate.y > 0.1f)
	{
		kineB->useGravity = true;
	}
	else if (gameObject->GetTransform()->translate.y < -0.1f)
	{
		kineB->useGravity = false;
		kineB->ApplyForce(Vector3(0, 1,0));
	}
	else
	{
		gameObject->GetTransform()->translate.y = 0;
		kineB->ResetVel(0, 1);
		kineB->useGravity = false;
	}
	
	if (gameObject->GetScript<AI>()->GetHealth() <= 0)
	{
		gameObject->ActiveSelf(false);
		gameObject->GetParent()->GetParent()->GetChild<GameObj>("SpawnerGO")->GetScript<Spawner>()->SetEnemyCount(gameObject->GetParent()->GetParent()->GetChild<GameObj>("SpawnerGO")->GetScript<Spawner>()->GetEnemyCount()- 1);
	}

	kineB->UpdateSuvat(dt);
	kineB->ResetForce();

	Node::Update(dt);
}

void Strategy::End()
{
	Node::End();
}

void Strategy::SetPlayerTrans(Vector3 trans)
{
	playerTrans = trans;
}

float Strategy::CalDis(Vector3 dest, Vector3 enemyPos)
{
	return ((dest.x - enemyPos.x) * (dest.x - enemyPos.x) + (dest.y - enemyPos.y) * (dest.y - enemyPos.y));
}

void Strategy::SetEnemyName(std::string enemyNames)
{
	this->enemyNames = enemyNames;
}

//void Strategy::SetState(Strategy::CURRENT_STATE enemyState)
//{
//	currentState = enemyState;
//}
