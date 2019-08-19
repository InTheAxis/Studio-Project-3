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
	health = 5;

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
		->SetAnimation(10, 8, 0.5f, 1);
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
	
	//if (direction == 1)
	//	TryChangeState(PLAYER_STATE::IDLE_R);
	//else if (direction == -1)
	//	TryChangeState(PLAYER_STATE::IDLE_L);
	if (attackTimer > 0)  //Attack Animation with timer 
	{
		if (direction == 1)
		{
			TryChangeState(PLAYER_STATE::ATTACK_R);
		}
		else if (direction == -1)
		{
			TryChangeState(PLAYER_STATE::ATTACK_L);
		}
	}
	else
	{
		if (direction == 1)
		{
			TryChangeState(PLAYER_STATE::IDLE_R);
		}
		else if (direction == -1)
		{
			TryChangeState(PLAYER_STATE::IDLE_L);
		}
	}

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
	if (kb->IsKeyPressed('R'))
	{
		if (direction == 1)
		{
			health--;
			TryChangeState(PLAYER_STATE::HIT_R);
		}
		else if (direction == -1)
		{
			health--;
			TryChangeState(PLAYER_STATE::HIT_L);
		}
		else
		{
			if (direction == 1)
				TryChangeState(PLAYER_STATE::IDLE_R);
			else if (direction == -1)
				TryChangeState(PLAYER_STATE::IDLE_L);
		}
	}

	if (kb->IsKeyPressed(VK_SPACE))  // Play Attack Animation 
	{			
		attackTimer = 1;
	}
	attackTimer -= dt;

	if (attackTimer < 0) 
		attackTimer = 0;
	
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
	
	if (gameObject->GetTransform()->translate.y >  0) //falling & jumping | gameObject->GetTransform()->translate.y >  worldHeight
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
	case PLAYER_STATE::ATTACK_L:
	case PLAYER_STATE::ATTACK_R:
		
		break;
	case PLAYER_STATE::HIT_L:
	case PLAYER_STATE::HIT_R:
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
	case PLAYER_STATE::HIT_L:
		Debug::Log("I am HIT_L");
		break;
	case PLAYER_STATE::HIT_R:
		Debug::Log("I am HIT_R");
		break;
	}
}

void Player::SetHeight(float groudheight)
{
	worldHeight = groudheight;
}