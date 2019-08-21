#include "PlayerController.h"
#include "../Manager/MgrGameObj.h"
#include "../../Node/GameObj.h"
#include "../../Node/Components/Sprite.h"
#include "../../Node/Components/Transform.h"
#include "../../Node/Components/KinemeticBody.h"
#include "../../Node/Components/Collider.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../../Utility/Input/ControllerMouse.h"
#include "../../Utility/Math/Spline.h"
#include "../../Node/Scripts/SkillTree.h"
#include "../../Node/Scripts/Achievements.h"

PlayerController::PlayerController(std::string name)
	: Node(name)
	, terrain(nullptr)
{
}

PlayerController::~PlayerController()
{
}

void test(ColInfo info)
{
	Debug::Log(info.other->GetName());
}

void PlayerController::Start()
{
	gameObject->GetTransform()->translate.Set(-1, 1, 0);

	sprite = AddChild<Sprite>()
		->SetAnimation(0, 8, 0.5f, 1)
		->SetAnimation(1, 8, 0.5f, 1)
		->SetAnimation(2, 8, 0.5f, 1)
		->SetAnimation(3, 8, 0.5f, 1)
		->SetAnimation(4, 8, 0.5f, 1)
		->SetAnimation(5, 8, 0.5f, 1)
		->SetAnimation(6, 8, 0.5f, 1)
		->SetAnimation(7, 8, 0.5f, 1)
		->SetAnimation(8, 8, 0.5f, 1)
		->SetAnimation(9 ,8, 0.5f, 1)
		->SetAnimation(10, 8, 0.5f, 1)
		->SetAnimation(11, 8, 0.5f, 1)
		->SetAnimation(12, 8, 0.5f, 1);
	sprite->SetGameObj(gameObject);
	sprite->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("anim"));
	
	kinb = AddChild<KinemeticBody>();
	kinb->SetGameObj(gameObject);
	kinb->maxVel.Set(1, 1, 0);
	kinb->frictionCoeff = 5;
	kinb->gravScale = 5;
	kinb->useGravity = false;

	attackRight = AddChild<Collider>("r");
	attackRight->SetGameObj(gameObject);
	attackRight->CreateAABB(Vector3(0, -0.5f), Vector3(0.5f, 0.5f));
	attackRight->isTrigger = true;
	attackRight->ActiveSelf(false);
	attackLeft = AddChild<Collider>("l");
	attackLeft->SetGameObj(gameObject);
	attackLeft->CreateAABB(Vector3(-0.5f, -0.5f), Vector3(0, 0.5f));
	attackLeft->isTrigger = true;
	attackLeft->ActiveSelf(false);
	attackAir = AddChild<Collider>("a");
	attackAir->SetGameObj(gameObject);
	attackAir->CreateAABB(Vector3(-0.5f, -0.5f), Vector3(0.5f, 0.5f));
	attackAir->isTrigger = true;
	attackAir->ActiveSelf(false);
	hitbox = AddChild<Collider>("h");
	hitbox->SetGameObj(gameObject);
	hitbox->CreateAABB(Vector3(-0.5f, -0.5f), Vector3(0.5f, 0.5f));	

	currState = nextState = P_STATE::IDLE_R;

	moveSpeed.Set(10, 30, 0);
	direction = 1;
	jumpTimer = attackTimer = hitTimer = deadTimer = 0.0;	
	health = 5;

	walking = false;

	Achievements::Instance()->setKnibRefrence(kinb);
	Node::Start();
}

void PlayerController::Update(double dt)
{
	ControllerKeyboard* kb = ControllerKeyboard::Instance();
	ControllerMouse* m = ControllerMouse::Instance();
	Vector3 input;
	float dtf = static_cast<float>(dt);
	
	//get input
	if (kb->IsKeyDown('A') && CanMove())
	{
		input.x = -1;
		walking = true;
	}
	else if (kb->IsKeyDown('D') && CanMove())
	{
		input.x = 1;
		walking = true;
	}
	else
	{
		input.x = 0;
		walking = false;
	}
	if (walking)
	{
		Achievements::Instance()->GetWalkTime(0.1);
	}

	if ((kb->IsKeyDown(VK_SPACE) || kb->IsKeyDown('W')) && (jumpTimer > 0 || (OnGround(0.1f) && CanMove())) && jumpTimer < 0.3)
	{
		Achievements::Instance()->GetJumpTimes(1);
		jumpTimer += dt;
	}
	else
		jumpTimer = 0;
	if ((m->IsButtonPressed(0) || kb->IsKeyPressed('Z')) && !attackTimer)
	{
		Achievements::Instance()->GetAttacTimes(1);
		attackTimer = 0.3f;
	}
	if (kb->IsKeyPressed('Q')) // TO BE PUT IN ENEMYSIDE TO DETECT IF ENEMY IS DEAD 
	{
		Achievements::Instance()->GetEnemyKilled(1);
	}

	if (kb->IsKeyPressed(VK_LCONTROL))
		TakeDamage(1);

	if (input.x != 0)
		direction = input.x;

	//apply movement
	if (OnGround(0.1f))
	{
		if (!input.x || !CanMove())
			Friction();
		else
			Move(input.x);
	}

	if (jumpTimer > 0)
		Jump();
	else
		Constrain();

	Fall();

	kinb->UpdateSuvat(dt);
	kinb->ResetForce();

	//apply attacks
	Attack(dt);

	//apply damage
	Hit(dt);
	Die(dt);


	ChangeState();

	// achievemets
	// kinb->maxVel.Set(Achievements::Instance()->maxValX, Achievements::Instance()->maxValY, 0);
	Debug::Log(kinb->maxVel);
	
	Node::Update(dt);
}

void PlayerController::End()
{
	Node::End();
}

void PlayerController::TryChangeState(P_STATE state)
{
	if (state == nextState)
		return;
	//check if valid to change state here
	this->nextState = state;
}

void PlayerController::ChangeState()
{
	if (currState == nextState)
		return;
	
	currState = nextState;
	sprite->SwitchAnimation((int)currState)->PlayAnimation();
	PrintState();
}

bool PlayerController::CanMove()
{
	return attackTimer <= 0 && deadTimer <= 0 && hitTimer <= 0;
}

bool PlayerController::OnGround(float offset, bool exact)
{
	if (exact)
		return Math::FIsEqual(gameObject->GetTransform()->translate.y, GetTerrainHeight() + offset);
	//else
	return (gameObject->GetTransform()->translate.y <= GetTerrainHeight() + offset);
}

void PlayerController::Move(float inputX)
{
	//if (Achievements::Instance()->walkAch(true))
	//	speedincrease = 10;
	kinb->ApplyForce(Vector3(inputX * moveSpeed.x /*+ speedincrease*/ * (OnGround() ? 1 : 0.3f), 0, 0));
	if (direction > 0)
		TryChangeState(P_STATE::MOVE_R);
	else if (direction < 0)
		TryChangeState(P_STATE::MOVE_L);	
}

void PlayerController::Friction()
{
	if (direction * kinb->GetVel().x < 0 || Math::FIsEqual(kinb->GetVel().x, 0))
		kinb->ResetVel(1, 0);
	else
		kinb->ApplyForce(Vector3(-direction * kinb->mass * kinb->frictionCoeff, 0, 0));

	if (direction > 0)
		TryChangeState(P_STATE::IDLE_R);
	else if (direction < 0)
		TryChangeState(P_STATE::IDLE_L);
}

void PlayerController::Jump()
{	
	kinb->ApplyForce(Vector3(0, moveSpeed.y, 0));
	TryChangeState(P_STATE::JUMP);
}

void PlayerController::Fall()
{
	if (gameObject->GetTransform()->translate.y > GetTerrainHeight())
	{
		kinb->useGravity = true;
		if (kinb->GetVel().y < -1.f)
			TryChangeState(P_STATE::FALL);
	}
	else if (OnGround())
	{		
		gameObject->GetTransform()->translate.y = GetTerrainHeight();
		kinb->useGravity = false;
		kinb->ResetVel(0, 1);
	}
}

void PlayerController::Constrain()
{	
	if (OnGround() || gameObject->GetTransform()->translate.y < GetTerrainHeight() + 0.1f)
		gameObject->GetTransform()->translate.y = GetTerrainHeight();	
}

void PlayerController::Attack(double dt)
{
	if (attackTimer <= 0)
	{
		attackTimer = 0;
		attackAir->ActiveSelf(false);
		attackLeft->ActiveSelf(false);
		attackRight->ActiveSelf(false);
		return;
	}
	else	
		attackTimer -= dt;

	if (!OnGround(0.1f))
	{
		TryChangeState(P_STATE::AIR_ATTACK);		
		attackAir->ActiveSelf(true);
		
	}
	else if (direction == 1)
	{
		TryChangeState(P_STATE::ATTACK_R);
		attackRight->ActiveSelf(true);
	
	}
	else if (direction == -1)
	{
		TryChangeState(P_STATE::ATTACK_L);
		attackLeft->ActiveSelf(true);
	}
}

void PlayerController::Die(double dt)
{
	if (deadTimer <= 0)
	{
		deadTimer = 0;
		return;
	}
	else
		deadTimer -= dt;

	if (direction == 1)
		TryChangeState(P_STATE::DYING_R);
	else if (direction == -1)
		TryChangeState(P_STATE::DYING_L);	
}

void PlayerController::Hit(double dt)
{
	if (hitTimer <= 0)
	{
		hitTimer = 0;
		return;
	}
	else
		hitTimer -= dt;

	if (direction == 1)
		TryChangeState(P_STATE::HIT_R);
	else if (direction == -1)
		TryChangeState(P_STATE::HIT_L);
}

float PlayerController::GetTerrainHeight()
{
	//return 0;// gameObject->GetTransform()->translate.x;
	return terrain->Fn(gameObject->GetTransform()->translate.x);
}

PlayerController * PlayerController::SetTerrain(Spline * s)
{
	terrain = s;
	return this;
}

void PlayerController::TakeDamage(int dmg)
{

	health -= dmg;
	if (health <= 0)
		deadTimer = 5;
	else
		hitTimer = 0.3f;
}
int PlayerController::DamageDealt()
{
	if (Achievements::Instance()->attackAch(true))
		damage = 2;
	return damage;
}

void PlayerController::PrintState()
{
	switch (currState)
	{
	case P_STATE::IDLE_R:
		Debug::Log("I am IDLE_R");
		break;
	case P_STATE::IDLE_L:
		Debug::Log("I am IDLE_L");
		break;
	case P_STATE::MOVE_L:
		Debug::Log("I am MOVE_L");
		break;
	case P_STATE::MOVE_R:
		Debug::Log("I am MOVE_R");
		break;
	case P_STATE::JUMP:
		Debug::Log("I am JUMP");
		break;
	case P_STATE::FALL:
		Debug::Log("I am FALL");
		break;
	case P_STATE::ATTACK_L:
		Debug::Log("I am ATTACK_L");
		break;
	case P_STATE::ATTACK_R:
		Debug::Log("I am ATTACK_R");
		break;
	case P_STATE::AIR_ATTACK:
		Debug::Log("I am AIR_ATTACK");
		break;
	case P_STATE::HIT_L:
		Debug::Log("I am HIT_L");
		break;
	case P_STATE::HIT_R:
		Debug::Log("I am HIT_R");
		break;
	case P_STATE::DYING_L:
		Debug::Log("I am DYING_L");
		break;
	case P_STATE::DYING_R:
		Debug::Log("I am DYING_R");
		break;
	}
}
