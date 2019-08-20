#ifndef AI_H
#define AI_H

#include "../Node.h"
#include "../Scene.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Components.h"
#include "../Components/Sprite.h"
#include "../Scripts.h"
//#include "../Scripts/Strategy.h"
#include "../../Utility/Strategy/Strategy.h"
#include "../../Utility/Strategy/StrategyOne.h"

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
	void SetAtkSpeed(float atkSpeed);
	float GetAtkSpeed();
	void SetDamage(float damage);
	float GetDamage();
	void ChangeStrategy(Strategy* newStrategy, bool remove);
	bool IsDead();

private:

	enum Wave //Use to activate the enemies in that wave
	{
		DEFAULT,
		WAVEONE,
		WAVETWO,
		WAVETHREE,
		WAVEFOUR,
		WAVEFIVE,
	};

	std::string enemyNames;
	Vector3 playerTrans;
	float health;
	float atkSpeed;
	float damage;
	float worldHeight;
	Strategy* strategy;
	KinemeticBody* kineB;
	Vector3 direction;
	int enemyCount;
	bool dead;
};

#endif