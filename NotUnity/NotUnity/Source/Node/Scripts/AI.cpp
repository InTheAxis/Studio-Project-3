#include "AI.h"
#include "../Manager/MgrGraphics.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../../Node/GameObj.h"
#include "../../Node/Components/Collider.h"
#include "../Scripts/Spawner.h"
#include "../Scripts/PlayerController.h"
#include "../Scripts/ColorSpot.h"
#include "Projectile.h"

AI::AI(std::string name) 
	: Node(name)
	, playerTrans(0.f, 0.f, 0.f)
	, damage(0.f)
	, strategy(nullptr)
	, direction(0.f,0.f,0.f)
	, dead(false)
	, enemyCount(0)
	, kineB(nullptr)
	, s(nullptr)
	, sat(1)
	, bounceTime(0)
	, bounceTimeTwo(0)
	, wave(0)
	, armour(0.f)
	, eNames("")
{
}

AI::~AI()
{
}

void AI::Start()
{
	sprite = AddChild<Sprite>();
	sprite->SetGameObj(gameObject);
	sprite->SetAnimation(0, 8, 0.5f, 1)
		->SwitchAnimation(0)
		->PlayAnimation()
		->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))
		->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial(eNames))
		->SelectShader(MgrGraphics::HSV_LIT)->SetRenderPass(RENDER_PASS::POST_FX);

	kineB = AddChild<KinemeticBody>();
	kineB->SetGameObj(gameObject);
	kineB->maxVel.Set(2, 2, 0);
	kineB->frictionCoeff = 0.2f;
	kineB->gravScale = 5;
	kineB->useGravity = false;

	for (int i = 0; i < ammoCount; ++i)
	{
		projectile[i] = AddChild<GameObj>("bull" + std::to_string(i))->AddScript<Projectile>();
		projectile[i]->SetTarget("player");
		projectile[i]->GetGameObj()->ActiveSelf(false);
	}

	if (gameObject->GetName()[0] == 'e')
		gameObject->GetScript<AI>()->health = 3;
	else
	{
		gameObject->GetScript<AI>()->health = 6;
	}

	Vector3 scale = gameObject->GetTransform()->scale;
	coll = AddChild<Collider>("c");
	coll->SetGameObj(gameObject);
	coll->CreateAABB(0.5f * scale.x);
	coll->tag = "enemy";
	
	trigger = AddChild<Collider>("t");
	trigger->SetGameObj(gameObject);
	trigger->isTrigger = true;
	trigger->CreateAABB(0.5f * scale.x);
	trigger->tag = "enemyA";

	colorSpot = AddChild<ColorSpot>();
	colorSpot->SetGameObj(gameObject);	

	t = gameObject->GetTransform();

	Node::Start();
}

void AI::Update(double dt)
{ 
	if (strategy != nullptr)
	{
		if (gameObject->GetName()[0] == 'b')
			strategy->Boss(true);
		else
			strategy->Boss(false);
	}

	sprite->SetAnimation(0, 8, 0.5f, 1)
		->SwitchAnimation(0)
		->PlayAnimation()
		->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))
		->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial(eNames))
		->SelectShader(MgrGraphics::HSV_LIT)->SetRenderPass(RENDER_PASS::POST_FX);

	if (!dead)
	{
		direction = (playerTrans - t->translate);
		if (!direction.IsZero())
			direction.Normalize();

		//Movement
		strategy->Update(playerTrans, gameObject->GetTransform()->translate, kineB, dt);

		//Attack
		Projectile* p = GetProjectile();
		strategy->Attack(p, gameObject->GetTransform()->translate, direction, dt);

		//For SBanana
		if (strategy->SelfInflict())
			health--;

		IfHealthZero();
		Gravity();
		kineB->UpdateSuvat(dt);
		kineB->ResetForce();		
	}
	else
	{
		t->translate.z = 0; //disable color spot
		if (m_lifetime > bounceTime + 0.5f)
			gameObject->ActiveSelf(false);
		if (m_lifetime > bounceTimeTwo + 0.5f)
			gameObject->ActiveSelf(false);
	}

	sat = Math::Max(0.f,  health / 3.f);
	colorSpot->radius = t->scale.x * 2 * (health / 3.f);

	sprite->SetHSV(-1, sat, -1);

	Node::Update(dt);
}

void AI::End()
{
	Node::End();
}

void AI::OnEnable()
{
	coll->OnCollideStay.Subscribe(&AI::HandleColl, this, "coll");
	trigger->OnCollideEnter.Subscribe(&AI::HandleColl, this, "tcoll");
}

void AI::OnDisable()
{
	if (coll)
		coll->OnCollideStay.UnSubscribe("coll");
	if (trigger)
		trigger->OnCollideEnter.UnSubscribe("tcoll");
}

void AI::SetPlayerTrans(Vector3 trans)
{
	playerTrans = trans;
}

void AI::SetHealth(float health)
{
	this->health = health;
}

float AI::GetHealth()
{
	return health;
}

void AI::SetDamageDealt(float damage)
{
	this->damage = damage;
}

float AI::GetDamageDealt()
{
	return damage;
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

void AI::SetSaturation(float sat)
{
	this->sat = sat;
}

ColorSpot* AI::GetColorSpot() const
{
	return colorSpot;
}

void AI::Reset()
{
	health = 3;
	armour = 3;
	sat = 1;
	dead = false;
	bounceTime = 0;
	bounceTimeTwo = 0;
	ResetBullets();
	ResetColorSpots();
}

void AI::Gravity()
{
	if (t->translate.y > GetWorldHeight() + 0.2f)
	{
		kineB->useGravity = true;
	}
	else
	{
		t->translate.y = GetWorldHeight();
		kineB->useGravity = false;
		kineB->ResetVel(0, 1);
	}
}

void AI::SetWave(int wave)
{
	this->wave = wave;
}

void AI::SetStrategy(Strategy* strat)
{
	strategy = strat;
}

void AI::IfHealthZero()
{
	if (health <= 0)
	{
		health = 0;
		dead = true;
	}
}

void AI::ResetBullets()
{
	for (int i = 0; i < ammoCount; ++i)
	{
		if (projectile[i])
			projectile[i]->GetGameObj()->ActiveSelf(false);
	}
}

void AI::ResetColorSpots()
{
	t->translate.z = 0;  //disable color spot
	colorSpot->radius = t->scale.x * 2;
}

void AI::SetDead(bool dead)
{
	this->dead = dead;
}

void AI::SetName(std::string eNames)
{
	this->eNames = eNames;
}

float AI::GetWorldHeight()
{
	return s->Fn(t->translate.x);
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

void AI::HandleColl(ColInfo info)
{
	if (info.other->GetGameObj()->GetScript<Projectile>())
		return;
	if (info.other->GetGameObj()->GetScript<PlayerController>())
	{
		AI* ai = info.coll->GetGameObj()->GetScript<AI>();
		if (info.other->isTrigger)
		{
			if (!ai->dead && ai->m_lifetime > ai->bounceTime + 0.5)
			{
				ai->bounceTime = ai->m_lifetime;

				if (ai->strategy->HasArmor())
				{
					ai->armour--;

					if (ai->armour <= 0)
					{
						ai->armour = 0;
						ai->health--;
					}
				}
				else
					ai->health--;

				IfHealthZero();
			}
		}
		else
		{
			if (!ai->dead && ai->m_lifetime > ai->bounceTimeTwo + 0.5)
			{
				ai->bounceTimeTwo = (ai->m_lifetime * 1.2f);
				trigger->isTrigger = true;
			}
			else
				trigger->isTrigger = false;
		}
	}
}