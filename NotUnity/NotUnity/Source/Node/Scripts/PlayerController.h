#ifndef PLAYER_CONTROL_H
#define PLAYER_CONTROL_H

//refactored player.h and player.cpp by terence

#include <sstream>

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Scripts.h"
#include "../Components.h"
#include "../../Utility/Math/Vector3.h"
#include "../../Node/Scene.h"

enum class P_STATE
{
	IDLE = 0,
	WALK,
	JUMP,
	FALL, 
	ATTACK,
	AIR_ATTACK,
	HIT,
	DYING,
	CHERR,
};

class KinemeticBody;
class Sprite;
class ColInfo;
class Collider;
class Spline;
class ColorSpot;
class Camera;
class PlayerController : public Node, public TypeID<PlayerController>, public Component
{
public:
	PlayerController(std::string name = "PlayerController");
	~PlayerController();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
	
	void OnEnable();
	void OnDisable();

	PlayerController* SetTerrain(Spline * s);	
	void TakeDamage(int dmg);	
	int DamageDealt();
	PlayerController* SetColorSpotRad(float radius);
	PlayerController* SetCameraRef(Camera* camera);

	bool IsDead();
	void Reset();


	void SetHealth(int h);
	int	GetHealth();

private:
	Transform* t, *swordT;
	KinemeticBody* kinb;
	Sprite* sprite, *swordSprite;
	Collider* attackRight, *attackLeft, *attackAir;
	Collider* hitbox;
	Spline* terrain;
	ColorSpot* colorSpot;
	Camera* camera;

	P_STATE currState, nextState;

	Vector3 moveSpeed;
	int direction;
	double jumpTimer, attackTimer, hitTimer, deadTimer;	
	int health, maxHealth;
	int damage;
    int speedincrease;

	bool walking;

	void TryChangeState(P_STATE state);
	void ChangeState();

	float GetTerrainHeight();

	bool CanMove();
	bool addHealth;
	bool OnGround(float offset = 0, bool exact = false);
	
	void Move(float inputX);
	void Friction();
	void Jump();
	void Fall();
	void Constrain();
	void Attack(double dt);
	void Die(double dt);
	void Hit(double dt);

	void PrintState();

	void HandleCollision(ColInfo info);
};

#endif