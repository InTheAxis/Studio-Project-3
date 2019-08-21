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

class AI : public Node, public TypeID<AI>, public Component
{
public:
	AI(std::string name = "AI");
	~AI();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	void SetPlayerTrans(Vector3 trans);
	void SetHealth(float health);
	float GetHealth();
	void SetDamage(float damage);
	float GetDamage();
	void ChangeStrategy(Strategy* newStrategy, bool remove);
	bool IsDead();
	AI* SetTerrain(Spline* s);

private:
	Vector3 playerTrans;
	float health;
	float damage;

	Vector3 direction;
	int enemyCount;
	bool dead;

	Strategy* strategy;
	KinemeticBody* kineB;
	Spline* s;

	float GetWorldHeight();
};

#endif