#ifndef PLAYER_H
#define PLAYER_H

#include <sstream>

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Scripts.h"
#include "../Components.h"
#include "../../Utility/Math/Vector3.h"

enum class PLAYER_STATE
{
	IDLE_R = 0,
	IDLE_L,
	MOVE_L,
	MOVE_R,
	JUMP,
	FALL, 
	//TOADD
	ATTACK_L,
	ATTACK_R,
	AIR_ATTACK,
	HIT_L,
	HIT_R,
	DYING_L,
	DYING_R,
};

class KinemeticBody;
class MgrGraphics;
class Sprite;
class Player : public Node, public TypeID<Player>, public Component
{
public:
	Player(std::string name = "Player");
	~Player();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
	
private:
	MgrGraphics* mg;
	KinemeticBody* kinb;
	Sprite* sprite;
	
	PLAYER_STATE currState, nextState;

	Vector3 moveSpeed;
	int direction;

	double jumpTimer;
	double attackTimer;

	bool attackCoolDown;

	void TryChangeState(PLAYER_STATE state);
	void ChangeState();
};

#endif