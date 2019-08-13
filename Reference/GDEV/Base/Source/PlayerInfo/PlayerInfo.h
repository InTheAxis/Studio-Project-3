#pragma once
#include "Vector3.h"
#include "Utility.h"
#include "../FPSCamera.h"
#include "../GroundEntity.h"
#include "../WeaponInfo/WeaponInfo.h"
#include "Collider/Collider.h"

class CPlayerInfo : public CCollider
{
public:
	enum MOVEMENT_STATE
	{
		IDLE = 0,
		MOVE,
		JUMP,
		FALL,
		CROUCH,
		PRONE,
		SPRINT,
		NUM_STATES,
	};

public:
	CPlayerInfo(void);
	~CPlayerInfo(void);

	//initialise this class instance
	void Init(int i);
	void Reset(void);


	//getter setters
	void SetPos(const Vector3& pos);
	void SetTarget(const Vector3& target);
	void SetUp(const Vector3& up);
	void SetBoundary(Vector3 max, Vector3 min);
	void SetTerrain(GroundEntity* m_pTerrain);
	void SetVelocity(Vector3 vel);
	void SetPlayerHeight(float addAmt = 0);
	void AddToMoveBy(const Vector3& moveBy);
	void ToggleSprinting(int set = -1);
	void ChangeWeapon(int idx);
	void SetMouseDelta(const Vector3& mouseDelta);
	void SetArrowKeyDelta(const Vector3& arrowKeyDelta);
	Vector3 GetPos(void) const;
	Vector3 GetTarget(void) const;
	Vector3 GetUp(void) const;
	Vector3 GetView(void) const;
	Vector3 GetRight(void) const;	
	Vector3 GetMoveBy(void) const;	
	bool GetSprinting(void) const;	
	MOVEMENT_STATE GetState(void) const;	
	CWeaponInfo* GetWeapon();
	int GetCurrentWeapon(void) const;
	int GetCurrentWeaponAmmo(void) const;
	int GetCurrentWeaponMaxAmmo(void) const;
	int GetCurrentWeaponClips(void) const;
	int GetCurrentWeaponMaxClips(void) const;
	float GetCameraSwayAngle(void) const;
	float GetMovementSpeed(void) const;
	bool GetShowScope();	

	//update
	void Update(double dt);

	//constrain the position within the borders
	void Constrain(void);
	//check if player is on terrain ground
	bool IsOnGround(void);

	//handling Camera
	void AttachCamera(FPSCamera* _cameraPtr);
	void DetachCamera();

	void RenderWeapon();
	void RenderHP();
	void RenderWeapInfo();

	void ChangeState(const MOVEMENT_STATE& target, bool force = false);

	void Shoot(bool leftClick);

	void TakeDamage(int amt = 1);
	void ResetHealth();

	void SetInertiaCam(bool b);
private:
	int playerNum;
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 position, target, up;
	Vector3 view, right;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;
	FPSCamera* attachedCamera;	

	//state trackers
	MOVEMENT_STATE m_mState;
	float playerHeight;
	bool sprinting;
	Vector3 moveBy;
	Vector3 mouseDelta;
	Vector3 arrowKeyDelta;
	
	//physics var
	double m_dSpeed;
	Vector3 m_vel;
	Vector3 m_gravity;
	float moveSpeed;

	//weapons
	int currWeapon;
	CWeaponInfo* weapons[2];	

	// Camera Sway
	float m_fCameraSwayAngle;
	float m_fCameraSwaySpeed;
	float m_fCameraSwayAngle_Limit; //The limits for left and right sway
	int m_cameraSwayDir;	//-1 = left, 1 = right, 0 inactive

	// Camera Inertia	
	float fAngularAcceleration;
	Vector3 fAngularVelocity;
	float fMaxAngularVelocity;
	float fResistance;

	float timeScale;
	bool showScope;

	int hp;
	const int maxHp = 50;
	
	bool inertiaCam;

	//movement helpers
	void Move(Vector3& position, const double dt, const Vector3& moveBy, const float speedMultiplier = 1.f);
	void UpdatePhysics(Vector3 & position, const double dt, const float speedMultiplier);	

	//state helpers
	void UpdateState(double dt);

	//camera helpers
	void CamRotateView(Vector3& view, const double dt, const float yaw, const float pitch, const float speedMultiplier = 1.0f);
	void CamRotateUp(Vector3& up, const double dt, const float roll, const float speedMultiplier = 1.0f);
	void UpdateTargetRightUp(Vector3& target, Vector3& right, Vector3& up);	
};
