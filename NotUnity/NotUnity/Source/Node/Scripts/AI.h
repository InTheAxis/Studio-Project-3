#ifndef AI_H
#define AI_H

#include "../Node.h"
#include "../Scene.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Components.h"
#include "../Components/Sprite.h"
#include "../Scripts.h"
#include "../../Utility/Strategy/Strategy.h"
#include "../../Utility/Strategy/STomato.h"
#include "../../Utility/Strategy/SCarrot.h"
#include "../../Utility/Strategy/SBanana.h"
#include "../../Utility/Strategy/SKiwi.h"
#include "../../Utility/Strategy/SBlueberry.h"
#include "../../Utility/Math/Spline.h"

class Projectile;
class Collider;
class ColInfo;
class ColorSpot;
class Transform;
class AI : public Node, public TypeID<AI>, public Component
{
public:
	AI(std::string name = "AI");
	~AI();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	void OnEnable();
	void OnDisable();

	void SetPlayerTrans(Vector3 trans);
	void SetHealth(float health);
	float GetHealth();
	void SetDamageDealt(float damage);
	float GetDamageDealt();
	bool IsDead();
	AI* SetTerrain(Spline* s);
	void SetSaturation(float sat);	
	ColorSpot* GetColorSpot() const;	

	void Reset();

	void Gravity();
	void SetWave(int wave);
	void SetStrategy(Strategy* strat);
	void IfHealthZero();
	void ResetBullets();
	void ResetColorSpots();
	void SetDead(bool dead);
	void SetAnimation(int num);
	void PlayAnimation();

private:
	Vector3 playerTrans;
	float health;
	float damage;
	float sat;
	float armour;

	double bounceTime;
	double bounceTimeTwo;

	Vector3 direction;
	int enemyCount;
	int wave;
	int enemyName;
	bool dead;
	const static int ammoCount = 3;

	Transform* t;
	Strategy* strategy;
	KinematicBody* kineB;
	Spline* s;
	Projectile* projectile[ammoCount];
	Collider* coll, *trigger;
	Sprite* sprite;

	STomato sTomato;
	SCarrot sCarrot;
	SBanana sBanana;
	SKiwi sKiwi;
	SBlueberry sBlueberry;
	ColorSpot* colorSpot;

	float GetWorldHeight();
	Projectile* GetProjectile();

	void HandleColl(ColInfo info);	
};

#endif