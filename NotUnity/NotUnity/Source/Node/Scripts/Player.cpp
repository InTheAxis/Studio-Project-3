#include "Player.h"
#include "../Manager/MgrGameObj.h"
#include "../../Node/GameObj.h"
#include "../../Node/Components/Sprite.h"
#include "../../Node/Components/KinemeticBody.h"
#include "../../Utility/Input//ControllerKeyboard.h"


Player::Player(std::string name) 
	: Node(name)
	, mg(nullptr)
{
}

Player::~Player()
{
}

void Player::Start()
{
	mg = MgrGraphics::Instance();
	gameObject->GetTransform()->translate.Set(-1, 1, 0);

	sprite = AddChild<Sprite>()
		->SetAnimation(0, 6, 1, 1)
		->SetAnimation(1, 6, 1, 1)
		->SetAnimation(2, 6, 1, 1)
		->SetAnimation(3, 6, 1, 1)
		->SetAnimation(4, 6, 1, 1)
		->SetAnimation(5, 6, 1, 1)
		->SetAnimation(6, 6, 1, 1)
		->SetAnimation(7, 6, 1, 1);
	sprite->SetGameObj(gameObject);
	sprite->AttachMesh(mg->GetCachedMesh("plane"))->AttachMaterial(mg->GetCachedMaterial("anim"));
	
	kinb = gameObject->AddComp<KinemeticBody>();
	kinb->maxVel.Set(2, 2, 0);
	kinb->frictionCoeff = 32;
	kinb->gravScale = 5;

	currState = nextState = PLAYER_STATE::IDLE_R;

	direction = 1;
	moveSpeed.Set(10, 15, 0);

	attackTimer = 0.0;
	jumpTimer = 0.0;
	Node::Start();
}

void Player::Update(double dt)
{
	ControllerKeyboard * kb = ControllerKeyboard::Instance();
	Vector3 input;
	float dtf = static_cast<float>(dt);
	
	if (direction == 1)
		TryChangeState(PLAYER_STATE::IDLE_R);
	else if (direction == -1)
		TryChangeState(PLAYER_STATE::IDLE_L);

	//get directiom to move in
	if (kb->IsKeyDown(VK_RIGHT))
	{
		input.x = 1;
		TryChangeState(PLAYER_STATE::MOVE_R);
		
	}
	if (kb->IsKeyDown(VK_LEFT))
	{
		input.x = -1;
		TryChangeState(PLAYER_STATE::MOVE_L);
	
	}
	// will play the animation when the user has press spacebar but theres a problem when you release it before the animation plays finish. !! need fix
	if (kb->IsKeyDown(VK_SPACE) && !attackCoolDown)
	{
		if (direction == 1)
		{
			attackTimer += dt;
			if (attackTimer > 0.5f)
			{
				attackCoolDown = true;
				attackTimer = 0;
			}
			TryChangeState(PLAYER_STATE::ATTACK_R);
		}
		else if (direction == -1)
		{
			attackTimer += dt;
			if (attackTimer > 0.5f)
			{
				attackCoolDown = true;
				attackTimer = 0;
			}
			TryChangeState(PLAYER_STATE::ATTACK_L);
		}
	}
	else
	{
		attackCoolDown = false;
	}
	
	if (kb->IsKeyDown(VK_UP))
	{
		input.y = (int)(jumpTimer < 0.3);
		jumpTimer += dt;
	}
	else
		jumpTimer = 0.0;
	//if (jumpTimer < 0.3 && nextState == PLAYER_STATE::JUMP)
	//{
	//	if (kb->IsKeyDown(VK_SPACE))
	//	{
	//		TryChangeState(PLAYER_STATE::AIR_ATTACK);
	//	}
	//}
	//falling & jumping
	if (gameObject->GetTransform()->translate.y >  0)
	{
		if (kinb->GetVel().y > 0)
			TryChangeState(PLAYER_STATE::JUMP);
		else
			TryChangeState(PLAYER_STATE::FALL);
		kinb->useGravity = true;
		kinb->ApplyForce(input.Scale(Vector3(moveSpeed.x * 0.1f, moveSpeed.y, moveSpeed.z)));
	}
	else
	{
		gameObject->GetTransform()->translate.y = 0;
		kinb->ResetVel(0, 1);
		kinb->useGravity = false;
		kinb->ApplyForce(input.Scale(moveSpeed));
	}

	if (input.x != 0) //set dir
		direction = input.x;
	else //apply friction
	{		
		if (direction * kinb->GetVel().x < 0)
			kinb->ResetVel(1, 0);
		else if (Math::FIsEqual(kinb->GetVel().x, 0))
			kinb->ResetVel(1, 0);
		else
			kinb->ApplyForce(-direction * kinb->frictionCoeff * kinb->mass * 0.1f);
	}	

	kinb->UpdateSuvat(dt);
	kinb->ResetForce();

	ChangeState();
	
	Node::Update(dt);
}

void Player::End()
{
	Node::End();
}

void Player::TryChangeState(PLAYER_STATE state)
{
	if (state == nextState)
		return;
	
	switch (state)
	{
	case PLAYER_STATE::IDLE_R:
	case PLAYER_STATE::IDLE_L:
		break;
	case PLAYER_STATE::MOVE_L:
	case PLAYER_STATE::MOVE_R:
		if (nextState == PLAYER_STATE::FALL || nextState == PLAYER_STATE::JUMP)
			return;
		break;
	case PLAYER_STATE::JUMP:
		break;
	case PLAYER_STATE::FALL:
		break;
	case PLAYER_STATE::ATTACK_L:
		break;
	case PLAYER_STATE::ATTACK_R:
		break;
	case PLAYER_STATE::AIR_ATTACK:
		break;

	}
	this->nextState = state;
}

void Player::ChangeState()
{
	if (currState == nextState)
		return;
	
	currState = nextState;
	sprite->SwitchAnimation((int)currState)->PlayAnimation();

	switch (currState)
	{
	case PLAYER_STATE::IDLE_R:
		Debug::Log("I am IDLE_R");
		break;
	case PLAYER_STATE::IDLE_L:
		Debug::Log("I am IDLE_L");
		break;
	case PLAYER_STATE::MOVE_L:
		Debug::Log("I am MOVE_L");
		break;
	case PLAYER_STATE::MOVE_R:
		Debug::Log("I am MOVE_R");
		break;
	case PLAYER_STATE::JUMP:
		Debug::Log("I am JUMP");
		break;
	case PLAYER_STATE::FALL:
		Debug::Log("I am FALL");
		break;
	case PLAYER_STATE::ATTACK_L:
		Debug::Log("I am ATTACK_L");
		break;
	case PLAYER_STATE::ATTACK_R:
		Debug::Log("I am ATTACK_R");
		break;
	case PLAYER_STATE::AIR_ATTACK:
		Debug::Log("I am AIR_ATTACK");
		break;
	}
}

