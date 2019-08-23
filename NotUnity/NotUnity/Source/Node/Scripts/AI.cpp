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
	//, health(0)
	, damage(0.f)
	, strategy(nullptr)
	, direction(0.f,0.f,0.f)
	, dead(false)
	, enemyCount(0)
	, kineB(nullptr)
	, s(nullptr)
	, sat(1)
	, interval(0)
	, bounceTime(0)
{
}

AI::~AI()
{
}

void AI::OnEnable()
{
	coll->OnCollideStay.Subscribe(&AI::HandleColl, this, "coll");		
}

void AI::OnDisable()
{
	if (coll)
		coll->OnCollideStay.UnSubscribe("coll");		
}

void AI::Start()
{
	sprite = AddChild<Sprite>();
	sprite->SetGameObj(gameObject);
	sprite->SetAnimation(0, 8, 0.5f, 1)
		->SwitchAnimation(0)
		->PlayAnimation()
		->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("plane"))
		->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("enemy"))
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

	if (gameObject->GetName()[0] == 'e' && strategy == NULL)
	{
		gameObject->GetScript<AI>()->health = 3;
		ChangeStrategy(new StrategyOne(), false);
	}
	if (gameObject->GetName()[0] == 'b' && strategy == NULL)
	{
		gameObject->GetScript<AI>()->health = 6;
		ChangeStrategy(new StrategyOne(), false);
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
	if (!dead)
	{
		interval += 1.f * static_cast<float>(dt);
		direction = (playerTrans - t->translate);
		if (!direction.IsZero())
			direction.Normalize();

		strategy->SetDest(playerTrans.x, playerTrans.y);
		strategy->Update(playerTrans, t->translate, dt);

		if (strategy->Attack() && interval >= 3.f)
		{
			Projectile* p = GetProjectile();
			if (p)
			{
				p->Discharge(t->translate, direction * 10);
				p->GetGameObj()->ActiveSelf(true);
			}
			interval = 0;
		}

		if ((playerTrans - t->translate).LengthSquared() > 3.f)
			kineB->ApplyForce(direction);
		else
			kineB->ResetVel(1, 0);

		if (t->translate.y > GetWorldHeight() + 0.1f)
		{
			kineB->useGravity = true;
		}
		else
		{
			t->translate.y = GetWorldHeight();
			kineB->useGravity = false;
			kineB->ResetVel(0, 1);
		}

		kineB->UpdateSuvat(dt);
		kineB->ResetForce();		
	}
	else
	{
		t->translate.z = 0; //disable color spot
		if (m_lifetime > bounceTime + 0.5f)
			gameObject->ActiveSelf(false);
	}

	sat = Math::Max(0.f,  health / 3.f);
	colorSpot->radius = t->scale.x * 2 * (health / 3.f);

	sprite->SetHSV(-1, sat, -1);

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

void AI::SetDamageDealt(float damage)
{
	this->damage = damage;
}

float AI::GetDamageDealt()
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

void AI::ResetBullets()
{
	for (int i = 0; i < ammoCount; ++i)
	{
		if (projectile[i])
			projectile[i]->GetGameObj()->ActiveSelf(false);
	}
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
	sat = 1;
	dead = false;
	bounceTime = 0;
	ResetBullets();
	ResetColorSpots();
}

void AI::ResetColorSpots()
{
	t->translate.z = 0;  //disable color spot
	colorSpot->radius = t->scale.x * 2;
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
	if (info.other->GetGameObj()->GetScript<PlayerController>() && info.other->isTrigger)
	{
		AI* ai = info.coll->GetGameObj()->GetScript<AI>();
		if (!ai->dead && ai->m_lifetime > ai->bounceTime + 0.5)
		{
			ai->bounceTime = ai->m_lifetime;
			ai->health--;

			if (ai->health <= 0)
			{
				ai->health = 0;
				ai->dead = true;
			}
		}
	}
}

void AI::SetPlayerTrans(Vector3 trans)
{
	playerTrans = trans;
}

void AI::End()
{
	Node::End();
}