#include "PlayerController.h"
#include "../Manager/MgrGameObj.h"
#include "../../Node/GameObj.h"
#include "../../Node/Components/Sprite.h"
#include "../../Node/Components/Transform.h"
#include "../../Node/Components/KinemeticBody.h"
#include "../../Node/Components/Collider.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../../Utility/Input/ControllerMouse.h"
#include "../../Node/Scripts/SkillTree.h"

PlayerController::PlayerController(std::string name)
	: Node(name)
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
	kinb->maxVel.Set(2, 2, 0);
	kinb->frictionCoeff = 5;
	kinb->gravScale = 5;
	kinb->useGravity = false;

	hitbox = AddChild<Collider>();
	hitbox->SetGameObj(gameObject);
	hitbox->CreateAABB(1);
	hitbox->isTrigger = true;
	hitbox->OnTrigger += &test;

	currState = nextState = P_STATE::IDLE_R;

	moveSpeed.Set(10, 30, 0);
	direction = 1;
	jumpTimer = attackTimer = hitTimer = deadTimer = 0.0;
	worldHeight = 0;
	health = 5;

	Node::Start();
}

void PlayerController::Update(double dt)
{
	ControllerKeyboard* kb = ControllerKeyboard::Instance();
	ControllerMouse* m = ControllerMouse::Instance();
	Vector3 input;
	float dtf = static_cast<float>(dt);
	
	//get input
	if (kb->IsKeyDown(VK_LEFT) && CanMove())
		input.x = -1;
	else if (kb->IsKeyDown(VK_RIGHT) && CanMove())
		input.x = 1;
	else
		input.x = 0;
	if ((kb->IsKeyDown(VK_SPACE) || kb->IsKeyDown(VK_UP)) && (jumpTimer > 0 || (OnGround() && CanMove())) && jumpTimer < 0.3)
		jumpTimer += dt;
	else
		jumpTimer = 0;
	if ((m->IsButtonPressed(0) || kb->IsKeyPressed('Z')) && !attackTimer)
		attackTimer = 0.3f;

	if (kb->IsKeyPressed(VK_LCONTROL))
		TakeDamage(1);

	if (input.x != 0)
		direction = input.x;

	//apply movement
	if (OnGround())
	{
		if (!input.x || !CanMove())
			Friction();
		else 
			Move(input.x);
	}
	if (jumpTimer > 0)
		Jump();
	Fall();

	kinb->UpdateSuvat(dt);
	kinb->ResetForce();

	//apply attacks
	Attack(dt);

	//apply damage
	Hit(dt);
	Die(dt);


	ChangeState();
	
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
		return Math::FIsEqual(gameObject->GetTransform()->translate.y, worldHeight + offset);
	//else
	return (gameObject->GetTransform()->translate.y <= worldHeight + offset);
}

void PlayerController::Move(float inputX)
{
	kinb->ApplyForce(Vector3(inputX * moveSpeed.x * (OnGround() ? 1 : 0.3f), 0, 0));
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
	if (gameObject->GetTransform()->translate.y > worldHeight)
	{
		kinb->useGravity = true;
		if (kinb->GetVel().y < 0)			
			TryChangeState(P_STATE::FALL);
	}
	else if (OnGround(-0.05f))
	{
		gameObject->GetTransform()->translate.y = worldHeight;
		kinb->ResetVel(0, 1);
		kinb->useGravity = false;
	}
}

void PlayerController::Attack(double dt)
{
	if (attackTimer <= 0)
	{
		attackTimer = 0;
		return;
	}
	else	
		attackTimer -= dt;

	if (!OnGround())
	{
		TryChangeState(P_STATE::AIR_ATTACK);
	}
	else if (direction == 1)
	{
		TryChangeState(P_STATE::ATTACK_R);
	}
	else if (direction == -1)
	{
		TryChangeState(P_STATE::ATTACK_L);
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

void PlayerController::SetHeight(float groudheight)
{
	if (gameObject->GetTransform()->translate.y < groudheight)
		gameObject->GetTransform()->translate.y = groudheight;
	worldHeight = groudheight;
}

void PlayerController::TakeDamage(int dmg)
{
	health -= dmg;
	if (health <= 0)
		deadTimer = 5;
	else
		hitTimer = 0.3f;
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
