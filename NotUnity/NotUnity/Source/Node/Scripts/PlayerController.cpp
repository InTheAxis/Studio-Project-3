#include "PlayerController.h"
#include "../Manager/MgrGameObj.h"
#include "../../Node/GameObj.h"
#include "../../Node/Components/Sprite.h"
#include "../../Node/Components/Transform.h"
#include "../../Node/Components/KinematicBody.h"
#include "../../Node/Components/Collider.h"
#include "../../Node/Components/Renderable.h"
#include "../../Node/Components/Camera.h"
#include "../../Node/Scripts/ColorSpot.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../../Utility/Input/ControllerMouse.h"
#include "../../Utility/Math/Spline.h"
#include "../Manager/MgrSkillTree.h"
#include "../Manager/MgrAchievements.h"

PlayerController::PlayerController(std::string name)
	: Node(name)
	, terrain(nullptr)

{
}

PlayerController::~PlayerController()
{
}

void PlayerController::Start()
{
	t = gameObject->GetTransform();
	t->translate.Set(-1, 1, 0.1f);

	sprite = AddChild<Sprite>()
		->SetAnimation(0, 8, 1.5f, 1) //idle
		->SetAnimation(1, 8, 0.5f, 1) //walk
		->SetAnimation(2, 8, 0.5f, 0) //jump
		->SetAnimation(3, 8, 0.5f, 1) //fall
		->SetAnimation(4, 6, 0.5f, 0) //attack
		->SetAnimation(5, 8, 0.5f, 1) //air attack
		->SetAnimation(6, 8, 1.5f, 1) //hit
		->SetAnimation(7, 8, 3.5f, 0) //dying
		->SetAnimation(8, 8, 0.5f, 1) //cheer
		->SwitchAnimation(0)
		->PlayAnimation();
	sprite->SetGameObj(gameObject);
	sprite->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("player"));
	sprite->SetHSV(-1,1,-1)->SetRenderPass(RENDER_PASS::POST_FX)->SelectShader(MgrGraphics::HSV_LIT);
	sprite->ToggleCullFace(false);
	
	swordT = AddChild<GameObj>("sword")->GetTransform();
	swordSprite = GetChild<GameObj>("sword")->AddComp<Sprite>()
		->SetAnimation(0, 6, 0.3f, 1) //idle
		->SetAnimation(1, 6, 0.3f, 0) //attack
		->SetAnimation(2, 6, 0.3f, 1) //air attack
		->SwitchAnimation(0)
		->PlayAnimation();
	swordSprite->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("sword"));
	swordSprite->SetHSV(-1, 1, -1)->SetRenderPass(RENDER_PASS::POST_FX)->SelectShader(MgrGraphics::HSV_UNLIT);
	swordSprite->ToggleCullFace(false);	
	
	kinb = AddChild<KinematicBody>();
	kinb->SetGameObj(gameObject);
	kinb->maxVel.Set(1, 3, 0);
	kinb->frictionCoeff = 5;
	kinb->gravScale = 10;
	kinb->useGravity = false;

	attackRight = AddChild<Collider>("r");
	attackRight->SetGameObj(gameObject);
	attackRight->CreateAABB(0.7f);
	attackRight->isTrigger = true;
	attackRight->ActiveSelf(false);
	attackRight->tag = "playerAR";
	attackLeft = AddChild<Collider>("l");
	attackLeft->SetGameObj(gameObject);
	attackLeft->CreateAABB(0.7f);
	attackLeft->isTrigger = true;
	attackLeft->ActiveSelf(false);
	attackLeft->tag = "playerAL";
	attackAir = AddChild<Collider>("a");
	attackAir->SetGameObj(gameObject);
	attackAir->CreateAABB(0.8f);
	attackAir->isTrigger = true;
	attackAir->ActiveSelf(false);
	hitbox = AddChild<Collider>("h");
	hitbox->SetGameObj(gameObject);
	hitbox->CreateAABB(0.5f);	
	hitbox->tag = "player";

	colorSpot = AddChild<ColorSpot>();
	colorSpot->SetGameObj(gameObject);

	maxHealth = 20 + MgrAchievements::Instance()->GetBonusHealth();
	moveSpeed.Set(10, 40, 0);
	addHealth = false;
	ActiveSelf(true);
	Reset();

	MgrAchievements::Instance()->setKnibRefrence(kinb);
	Node::Start();
}

void PlayerController::Update(double dt)
{
	ControllerKeyboard* kb = ControllerKeyboard::Instance();
	ControllerMouse* m = ControllerMouse::Instance();
	Vector3 input;
	float dtf = static_cast<float>(dt);

	if (Math::FIsEqual(dtf, 0.f))
		return;

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
		MgrAchievements::Instance()->SetWalkTime(0.1f);
	}

	if ((kb->IsKeyDown(VK_SPACE) || kb->IsKeyDown('W')) && (jumpTimer > 0 || (OnGround(0.1f) && CanMove())) && jumpTimer < 0.3)
	{
		MgrAchievements::Instance()->SetJumpTimes(1);
		jumpTimer += dt;
	}
	else
		jumpTimer = 0;
	if ((m->IsButtonPressed(0) || kb->IsKeyPressed('Z')) && !attackTimer)
	{
		MgrAchievements::Instance()->SetAttackTimes(1);
		attackTimer = 0.3f;
	}
	if (kb->IsKeyPressed('Q')) // TO BE PUT IN ENEMYSIDE TO DETECT IF ENEMY IS DEAD 
	{
		MgrAchievements::Instance()->GetEnemyKilled();
	}

	if (kb->IsKeyPressed(VK_F1))
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

	/*Debug::Log(health);*/
	
	//apply damage
	Hit(dt);
	Die(dt);

	ChangeState();

	//update colorSpot
	colorSpot->SetUniform(0);

	//update sword's transform
	swordT->translate = t->translate;
	swordT->scale.x = t->scale.x;
	if (t->scale.x < 0)
		swordT->translate += Vector3(-0.25f, 0.2f, -0.1f);
	else
		swordT->translate.z += 0.1f;

	// achievemets
	// kinb->maxVel.Set(Achievements::Instance()->maxValX, Achievements::Instance()->maxValY, 0);	

	Node::Update(dt);
}

void PlayerController::End()
{
	Node::End();
}

void PlayerController::OnEnable()
{
	hitbox->OnCollideStay.Subscribe(&PlayerController::HandleCollision, this, "coll");
	attackLeft->OnTriggerStay.Subscribe(&PlayerController::HandleTrigger, this, "trigL");
	attackRight->OnTriggerStay.Subscribe(&PlayerController::HandleTrigger, this, "trigR");
}

void PlayerController::OnDisable()
{
	if (hitbox)
		hitbox->OnCollideStay.UnSubscribe("coll");
	if (attackLeft)
		attackLeft->OnTriggerStay.UnSubscribe("trigL");
	if (attackRight)
		attackRight->OnTriggerStay.UnSubscribe("trigR");
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
	if (direction > 0)
		t->scale.x = 1;
	else if (direction < 0)
		t->scale.x = -1;

	if (currState == nextState)
		return;
	
	currState = nextState;
	sprite->SwitchAnimation((int)currState)->PlayAnimation();
	if (currState == P_STATE::ATTACK || currState == P_STATE::AIR_ATTACK)
		swordSprite->SwitchAnimation((int)currState - 3)->PlayAnimation();
	else
		swordSprite->SwitchAnimation(0)->PlayAnimation();
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

void PlayerController::SetHealth(int h)
{
	health = h;

}

int PlayerController::GetHealth()
{
	return health; 
}

void PlayerController::Move(float inputX)
{

	kinb->ApplyForce(Vector3(inputX * moveSpeed.x /*+ speedincrease*/ * (OnGround() ? 1 : 0.5f), 0, 0));
	TryChangeState(P_STATE::WALK);	
}

void PlayerController::Friction()
{
	if (direction * kinb->GetVel().x < 0 || Math::FIsEqual(kinb->GetVel().x, 0))
		kinb->ResetVel(1, 0);
	else
		kinb->ApplyForce(Vector3(-direction * kinb->mass * kinb->frictionCoeff, 0, 0));

	TryChangeState(P_STATE::IDLE);
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
	else
	{
		if (direction == 1)
			attackRight->ActiveSelf(true);
		else if (direction == -1)
			attackLeft->ActiveSelf(true);
		TryChangeState(P_STATE::ATTACK);
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

	TryChangeState(P_STATE::DYING);
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

	TryChangeState(P_STATE::HIT);
}

float PlayerController::GetTerrainHeight()
{
	//return 0;// gameObject->GetTransform()->translate.x;
	if (!terrain)
		Debug::LogError("Terrain missing :(");
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
	if (health <= 0 && deadTimer <= 0)
		deadTimer = 3.5f;
	else if (hitTimer <= 0)
		hitTimer = 0.3f;
	camera->Shake(0.05f, 0.15f);
}
int PlayerController::DamageDealt()
{
	if (MgrAchievements::Instance()->GetAttackLevel1())
		return damage = 2;
	else if (MgrAchievements::Instance()->GetAttackLevel2())
		return damage = 4;
	return damage = 1;
}

PlayerController* PlayerController::SetColorSpotRad(float radius)
{
	colorSpot->radius = radius;
	return this;
}

PlayerController * PlayerController::SetCameraRef(Camera * camera)
{
	this->camera = camera;
	return this;
}

bool PlayerController::IsDead()
{
	return deadTimer > 0;
}

void PlayerController::Reset()
{
	//gameObject->GetTransform()->translate.Set(-1, 1, 0);
	
	direction = 1;
	jumpTimer = attackTimer = hitTimer = deadTimer = 0.0;
	health = maxHealth;

	walking = false;

	TryChangeState(P_STATE::IDLE);
	ChangeState();
}

void PlayerController::PrintState()
{
	switch (currState)
	{
	case P_STATE::IDLE:
		Debug::Log("I am IDLE");
		break;
	case P_STATE::WALK:
		Debug::Log("I am MOVE");
		break;
	case P_STATE::JUMP:
		Debug::Log("I am JUMP");
		break;
	case P_STATE::FALL:
		Debug::Log("I am FALL");
		break;
	case P_STATE::ATTACK:
		Debug::Log("I am ATTACK");
		break;
	case P_STATE::AIR_ATTACK:
		Debug::Log("I am AIR_ATTACK");
		break;
	case P_STATE::HIT:
		Debug::Log("I am HIT");
		break;
	case P_STATE::DYING:
		Debug::Log("I am DYING");
		break;
	}
}

void PlayerController::HandleCollision(ColInfo info)
{
	if (info.other->tag == "bulletplayer")
	{
		TakeDamage(1);
		Debug::Log("hittttttttttttt");
	}

	if (info.other->tag == "enemyA" && info.other->isTrigger)
		TakeDamage(1);

	if (info.other->tag == "rock")
		info.other->GetGameObj()->GetComp<KinematicBody>()->ApplyImpulse(Vector3(10 * direction, 1, 0));
}

void PlayerController::HandleTrigger(ColInfo info)
{
	if (info.other->tag == "crate")
		info.other->GetGameObj()->GetTransform()->translate.y = 100;
}
