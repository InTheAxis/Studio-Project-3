#pragma once
#include "Vector3.h"
#include "../FPSCamera.h"
#include "../GroundEntity.h"
#include "../WeaponInfo/WeaponInfo.h"

class CPlayerInfo
{
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
protected:
	static CPlayerInfo *s_instance;
	CPlayerInfo(void);

public:
	static CPlayerInfo *GetInstance()
	{
		if (!s_instance)
			s_instance = new CPlayerInfo;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	~CPlayerInfo(void);

	//initialise this class instance
	void Init(void);
	void Reset(void);

	//getter setters
	void SetPos(const Vector3& pos);
	void SetTarget(const Vector3& target);
	void SetUp(const Vector3& up);
	void SetBoundary(Vector3 max, Vector3 min);
	void SetTerrain(GroundEntity* m_pTerrain);
	void SetVelocity(Vector3 vel);
	void SetPlayerHeight(float addAmt = 0);
	Vector3 GetPos(void) const;
	Vector3 GetTarget(void) const;
	Vector3 GetUp(void) const;
	
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

private:
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
	
	//physics var
	double m_dSpeed;
	Vector3 m_vel;
	Vector3 m_gravity;

	int currWeapon;
	CWeaponInfo* weapons[2];	

	//movement helpers
	void Move(Vector3& position, const double dt, const Vector3& moveBy, const float speedMultiplier = 1.f);
	void UpdatePhysics(Vector3 & position, const double dt, const float speedMultiplier);	

	//state helpers
	void ChangeState(MOVEMENT_STATE& mState, const MOVEMENT_STATE& target, bool force = false);
	void UpdateState(double dt);

	//camera helpers
	void CamRotate(Vector3& view, const double dt, const float yaw, const float pitch, const float speedMultiplier = 1.0f);
	void UpdateTargetRightUp(Vector3& target, Vector3& right, Vector3& up);	
};
