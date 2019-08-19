#ifndef AI_H
#define AI_H

#include "../Node.h"
#include "../Scene.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Components.h"
#include "../Components/Sprite.h"
#include "../Scripts.h"
#include "../Scripts/Strategy.h"

class AI : public Node, public TypeID<AI>, public Component
{
public:
	AI(std::string name = "AI");
	~AI();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
	void SetPlayerTrans(Vector3 trans);
	//void Move();
	void SetHealth(int health);
	float GetHealth();
	void SetAtkSpeed(int atkSpeed);
	float GetAtkSpeed();
	void SetDamage(int damage);
	float GetDamage();

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

	std::string EnemyNames; //Use to pass to and from different functions
	Vector3 playerTrans;
	float health;
	float atkSpeed;
	float damage;
};

#endif