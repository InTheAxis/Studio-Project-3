#ifndef STRATEGY_H
#define STRATEGY_H

#include "../Node.h"
#include "../Scene.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Components.h"
#include "../Components/Sprite.h"
#include "../Scripts.h"
#include "../Scripts/AI.h"
#include "../Components/KinemeticBody.h"
#include "../Scripts/Spawner.h"

class KinemeticBody;
class Strategy : public Node, public TypeID<Strategy>, public Component
{
public:
	Strategy(std::string name = "Strategy");
	~Strategy();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	void SetPlayerTrans(Vector3 trans); //Set from other cpp
	float CalDis(Vector3 dest, Vector3 enemyPos); //Calculate the distance between player and enemy
	//void SetDestination(const float x, const float y); //Make enemy move //playerTrans.x = x;
	//void SetState(Strategy::CURRENT_STATE);
	void SetEnemyName(std::string enemyNames);

	enum CURRENT_STATE //State of enemy
	{
		IDLE,
		ATTACK,
		REPEL,
		COLLIDE
	};

	enum STATE_RANGE //Range of enemy for state change
	{
		STATE_ATTACK = 25,
		STATE_REPEL = 2,
		STATE_COLLIDE = 1
	};

private:
	Vector3 playerTrans;
	Strategy::CURRENT_STATE currentState; //Current state of enemy
	KinemeticBody* kineB;
	std::string enemyNames;
};

#endif