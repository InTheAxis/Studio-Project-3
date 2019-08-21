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
#include "../../Utility/Strategy/StrategyOne.h"
#include "../../Utility/Math/Spline.h"

class Projectile;
class Collider;
class ColInfo;
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
	void ChangeStrategy(Strategy* newStrategy, bool remove);
	bool IsDead();
	AI* SetTerrain(Spline* s);
	void ResetBullets();



private:
	Vector3 playerTrans;
	float health;
	float damage;
	float sat;
	float interval;

	Vector3 direction;
	int enemyCount;
	bool dead;
	const static int ammoCount = 3;

	Strategy* strategy;
	KinemeticBody* kineB;
	Spline* s;
	Projectile* projectile[ammoCount];
	Collider* coll, *trigger;
	Sprite* sprite;

	float GetWorldHeight();
	Projectile* GetProjectile();

	friend void AIOnHit(ColInfo info);
	friend void AIOnAttack(ColInfo info);
};

#endif