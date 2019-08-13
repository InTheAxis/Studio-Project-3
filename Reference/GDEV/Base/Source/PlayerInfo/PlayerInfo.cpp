#include "PlayerInfo.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../Projectile/Projectile.h"
#include "../WeaponInfo/Pistol.h"
#include "../WeaponInfo/Hook.h"
#include "../WeaponInfo/AssaultRifle.h"
#include "../WeaponInfo/Shotgun.h"
#include "MatrixStack.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "RenderHelper.h"
#include "../WeaponInfo/AmmoDumpManager.h"
#include "../SoundEngine.h"

CPlayerInfo::CPlayerInfo(void)
	: attachedCamera(NULL)
	, m_pTerrain(NULL)	
	, playerHeight(0)
	, playerNum(0)
{	
	weapons[0] = nullptr;
	weapons[1] = nullptr;
}

CPlayerInfo::~CPlayerInfo(void)
{
	if (weapons[0])
	{
		delete weapons[0];
		weapons[0] = NULL;
	}
	if (weapons[1])
	{
		delete weapons[1];
		weapons[1] = NULL;
	}
	m_pTerrain = NULL;
}

// Initialise this class instance
void CPlayerInfo::Init(int i)
{	
	playerNum = i;

	//set the default values
	defaultPosition.Set(0,0,10);
	defaultTarget.Set(0,0,0);
	defaultUp.Set(0,1,0);

	//set the current values
	position.Set(0, 0, 10);
	view.Set(0, 0, -1);
	up = defaultUp;
	UpdateTargetRightUp(target, right, up);

	//set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	//set start state
	ChangeState(MOVEMENT_STATE::IDLE);
	playerHeight = 0;
	sprinting = false;
	moveBy.SetZero();
	mouseDelta.SetZero();

	//set physics var
	m_dSpeed = 1;
	m_gravity.Set(0, -9.8f, 0);
	m_vel.SetZero();
	moveSpeed = 40;	

	//set the pistol as the primary weapon
	switch (playerNum)
	{
	case 0:
		currWeapon = 0;
		weapons[0] = new CShotgun();
		weapons[0]->Init();
		weapons[0]->SetMesh(MeshBuilder::GetInstance()->GetMesh("PLAYER_SHOTGUN"));
		break;
	case 1:
		currWeapon = 0;
		weapons[0] = new CAssaultRifle();
		weapons[0]->Init();
		weapons[0]->SetMesh(MeshBuilder::GetInstance()->GetMesh("PLAYER_ASSAULT"));
		break;
	case 2:
		currWeapon = 0;
		weapons[0] = new CHook();
		weapons[0]->Init();
		weapons[0]->SetMesh(MeshBuilder::GetInstance()->GetMesh("PLAYER_HOOK"));
		weapons[1] = new CPistol();
		weapons[1]->Init();
		weapons[1]->SetMesh(MeshBuilder::GetInstance()->GetMesh("PLAYER_PISTOL"));
		break;
	}

	//initialise sway vars
	m_fCameraSwayAngle = 0.0f;
	m_fCameraSwaySpeed = 8.f;
	m_fCameraSwayAngle_Limit = 1.f;
	m_cameraSwayDir = 0;

	// Initialise the variables for camera inertia	
	fAngularAcceleration = 2.f;
	fAngularVelocity.SetZero();
	fMaxAngularVelocity = 2.5f;
	fResistance = 0.02f;

	timeScale = 1;
	showScope = false;

	hp = 0;

	inertiaCam = false;

	float scale = 2;
	SetAABB(Vector3(-scale, 0, -scale), Vector3(scale, 2 * scale, scale));
}

// Returns true if the player is on ground
bool CPlayerInfo::IsOnGround(void)
{		
	return (position.y == playerHeight);
}

// Set position
void CPlayerInfo::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CPlayerInfo::SetTarget(const Vector3& target)
{
	this->target = target;
}

// Set position
void CPlayerInfo::SetUp(const Vector3& up)
{
	this->up = up;
}

// Set the boundary for the player info
void CPlayerInfo::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Set the terrain for the player info
void CPlayerInfo::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());				
		SetPlayerHeight();
	}
}

void CPlayerInfo::SetVelocity(Vector3 vel)
{
	m_vel = vel;
}


void CPlayerInfo::SetPlayerHeight(float addAmt)
{
	playerHeight = m_pTerrain->GetTerrainHeight(position) + addAmt;

	if (position.y != playerHeight)
	{
		position.y = playerHeight;
	}
}

void CPlayerInfo::AddToMoveBy(const Vector3 & moveBy)
{
	this->moveBy += moveBy;
}

void CPlayerInfo::ToggleSprinting(int set)
{
	if (set < 0)
		sprinting = !sprinting;
	else
		sprinting = (bool)set;
	m_cameraSwayDir = (int)sprinting;
}

void CPlayerInfo::ChangeWeapon(int idx)
{
	//TODO error check
	if (weapons[idx])
		currWeapon = idx;
}

void CPlayerInfo::SetMouseDelta(const Vector3 & mouseDelta)
{
	this->mouseDelta = mouseDelta;
}

void CPlayerInfo::SetArrowKeyDelta(const Vector3 & arrowKeyDelta)
{
	this->arrowKeyDelta = arrowKeyDelta;
}

// Reset this player instance to default
void CPlayerInfo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;	
	view = target - position;
	right = view.Cross(defaultUp).Normalized();
	m_cameraSwayDir = 0;
	fAngularVelocity = 0;	
}

Vector3 CPlayerInfo::GetPos(void) const
{
	return position;
}
Vector3 CPlayerInfo::GetUp(void) const
{
	return up;
}
Vector3 CPlayerInfo::GetTarget(void) const
{
	return target;
}
Vector3 CPlayerInfo::GetView(void) const
{
	return view;
}
Vector3 CPlayerInfo::GetRight(void) const
{
	return right;
}
Vector3 CPlayerInfo::GetMoveBy(void) const
{
	return moveBy;
}
bool CPlayerInfo::GetSprinting(void) const
{
	return sprinting;
}
CPlayerInfo::MOVEMENT_STATE CPlayerInfo::GetState(void) const
{
	return m_mState;
}
CWeaponInfo* CPlayerInfo::GetWeapon()
{
	return weapons[currWeapon];
}
int CPlayerInfo::GetCurrentWeapon(void) const
{
	return currWeapon;
}
int CPlayerInfo::GetCurrentWeaponAmmo(void) const
{
	return weapons[currWeapon]->GetMagRound();
}
int CPlayerInfo::GetCurrentWeaponMaxAmmo(void) const
{
	return weapons[currWeapon]->GetMaxMagRound();
}
int CPlayerInfo::GetCurrentWeaponClips(void) const
{
	return weapons[currWeapon]->GetTotalClips();
}
int CPlayerInfo::GetCurrentWeaponMaxClips(void) const
{
	return weapons[currWeapon]->GetMaxTotalClips();
}
float CPlayerInfo::GetCameraSwayAngle(void) const
{
	return m_fCameraSwayAngle;
}

float CPlayerInfo::GetMovementSpeed(void) const
{
	return timeScale;
}

bool CPlayerInfo::GetShowScope()
{
	return showScope;
}

/********************************************************************************
 Hero Update
 ********************************************************************************/
void CPlayerInfo::Update(double dt)
{
	//update pos
	{		
		UpdateState(dt);		

		if (moveBy.IsZero() && IsOnGround())
			timeScale = 0.02f;
		else
		{
			if (sprinting)
				timeScale = 1.5f;
			else
				timeScale = 1.f;			
		}

		//update movement based on state
		switch (m_mState)
		{
		default:
			moveSpeed = Lerpf(moveSpeed, 40.f, 0.5f);
			break;
		case MOVEMENT_STATE::JUMP:
			moveSpeed = Lerpf(moveSpeed, 25.f, 0.5f);
			break;
		case MOVEMENT_STATE::CROUCH:
			moveSpeed = Lerpf(moveSpeed, 30.f, 0.5f);
			break;
		case MOVEMENT_STATE::PRONE:
			moveSpeed = Lerpf(moveSpeed, 20.f, 0.5f);
			break;	
		case MOVEMENT_STATE::SPRINT:
			moveSpeed = Lerpf(moveSpeed, 80.f, 0.05f);
			break;
		}		
		Move(position, dt, moveBy, moveSpeed);
		moveBy.SetZero();
	}	
	//constrain position within boundary
	Constrain();

	// Update camera inertia x
	if (fAngularVelocity.x != 0)
	{
		if (fAngularVelocity.x > 0.0f)
		{
			fAngularVelocity.x -= fResistance;
			if (fAngularVelocity.x < 0.0f) fAngularVelocity.x = 0.0f;
		}
		else if (fAngularVelocity.x < 0.0f)
		{
			fAngularVelocity.x += fResistance;
			if (fAngularVelocity.x > 0.0f) fAngularVelocity.x = 0.0f;
		}
	}		
	fAngularVelocity.x += mouseDelta.x * fAngularAcceleration * dt;	
	// Limit the angular velocity
	if (fAngularVelocity.x > fMaxAngularVelocity) fAngularVelocity.x = fMaxAngularVelocity;
	if (fAngularVelocity.x < -fMaxAngularVelocity) fAngularVelocity.x = -fMaxAngularVelocity;
	// Update camera inertia y
	if (fAngularVelocity.y != 0)
	{
		if (fAngularVelocity.y > 0.0f)
		{
			fAngularVelocity.y -= fResistance;
			if (fAngularVelocity.y < 0.0f) fAngularVelocity.y = 0.0f;
		}
		else if (fAngularVelocity.y < 0.0f)
		{
			fAngularVelocity.y += fResistance;
			if (fAngularVelocity.y > 0.0f) fAngularVelocity.y = 0.0f;
		}
	}
	fAngularVelocity.y += mouseDelta.y * fAngularAcceleration * dt;
	// Limit the angular velocity
	if (fAngularVelocity.y > fMaxAngularVelocity) fAngularVelocity.y = fMaxAngularVelocity;
	if (fAngularVelocity.y < -fMaxAngularVelocity) fAngularVelocity.y = -fMaxAngularVelocity;
	//camera sway
	if (m_cameraSwayDir == 0)
	{
		m_fCameraSwayAngle = 0;
		up = defaultUp;
	}
	else
	{
		m_fCameraSwayAngle += m_cameraSwayDir * m_fCameraSwaySpeed * dt;
		if (m_fCameraSwayAngle > m_fCameraSwayAngle_Limit)
		{
			m_fCameraSwayAngle = m_fCameraSwayAngle_Limit;
			m_cameraSwayDir = -1;
		}
		else if (m_fCameraSwayAngle < -m_fCameraSwayAngle_Limit)
		{
			m_fCameraSwayAngle = -m_fCameraSwayAngle_Limit;
			m_cameraSwayDir = 1;
		}
	}
	
	if (inertiaCam)	CamRotateView(view, dt, fAngularVelocity.x, fAngularVelocity.y, 100);
	else CamRotateView(view, dt, mouseDelta.x, mouseDelta.y, 100);
	//rotate with arrow keys	
	CamRotateView(view, dt, arrowKeyDelta.x, arrowKeyDelta.y, 100);
	//update target, right and up, normalized
	UpdateTargetRightUp(target, right, up);
	//cam sway
	CamRotateUp(up, dt, m_fCameraSwayAngle, 100);

	// If a camera is attached to this playerInfo class, then update it
	if (attachedCamera)
	{
		attachedCamera->SetCameraPos(position);
		attachedCamera->SetCameraTarget(target);
		attachedCamera->SetCameraUp(up);
	}

	if (weapons[currWeapon])
		weapons[currWeapon]->Update(dt);	

	// If the user presses P key, then reset the view to default values
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
	{
		Reset();
	}

	//hard coded :p
	if (playerNum == 1)
	{
		showScope = weapons[currWeapon]->ads;
		inertiaCam = weapons[currWeapon]->ads;
	}
}

// Constrain the position within the borders
void CPlayerInfo::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.y > maxBoundary.y - 1.0f)
		position.y = maxBoundary.y - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.y < minBoundary.y + 1.0f)
		position.y = minBoundary.y + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;
}

void CPlayerInfo::AttachCamera(FPSCamera* _cameraPtr)
{
	attachedCamera = _cameraPtr;
}

void CPlayerInfo::DetachCamera()
{
	attachedCamera = nullptr;
}

void CPlayerInfo::RenderWeapon()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();

	switch (playerNum)
	{
	case 0:
		switch (currWeapon)
		{
		case 0:
			if (!weapons[currWeapon]->ads)
			{
				modelStack.Translate(220, -200, -10);
				modelStack.Rotate(-75.f, 0, 1, 0);
				modelStack.Rotate(-5.f, 0, 0, 1);
				if (!weapons[currWeapon]->GetCanFire())
				{
					modelStack.Rotate(-5, 0, 0, 1);
				}
				modelStack.Scale(200, 200, 200);
			}
			else
			{
				modelStack.Translate(0, -200, 0);
				modelStack.Rotate(-90.f, 0, 1, 0);
				modelStack.Rotate(-5.f, 0, 0, 1);
				if (!weapons[currWeapon]->GetCanFire())
				{
					modelStack.Rotate(-5, 0, 0, 1);
				}
				modelStack.Scale(200, 200, 200);
			}
			break;
			break;
		}
		break;
	case 1:
		switch (currWeapon)
		{
		case 0:
			if (!weapons[currWeapon]->ads)
			{
				modelStack.Translate(220, -800, -10);
				modelStack.Rotate(-70.f, 0, 1, 0);
				modelStack.Rotate(-5.f, 0, 0, 1);
				if (!weapons[currWeapon]->GetCanFire())
				{
					modelStack.Rotate(-5, 0, 0, 1);
				}
				modelStack.Scale(300, 300, 300);
			}
			else
			{
				//modelStack.Translate(0, -800, 0);	
				//modelStack.Rotate(-90.f, 0, 1, 0);
				//modelStack.Rotate(-5.f, 0, 0, 1);
				//if (!weapons[currWeapon]->GetCanFire())
				//{
				//	modelStack.Rotate(-5, 0, 0, 1);
				//}
				//modelStack.Scale(300, 300, 300);
			}
			break;
		}
		break;
	case 2:
		switch (currWeapon)
		{
		case 0:
			modelStack.Translate(400, -230, -10);
			modelStack.Rotate(-30.f, 0, 1, 0);			
			modelStack.Rotate(-5.f, 1, 0, 0);
			if (!weapons[currWeapon]->GetCanFire())
			{
				modelStack.Rotate(-5, 1, 0, 0);
			}
			modelStack.Scale(40, 40, 40);
			break;
		case 1:
			modelStack.Translate(250, -300, 0);
			modelStack.Rotate(30.f, 0, 1, 0);
			modelStack.Rotate(5.f, 1, 0, 0);
			if (!weapons[currWeapon]->GetCanFire())
			{
				modelStack.Rotate(5, 1, 0, 0);
			}
			modelStack.Scale(100, 100, 100);
			break;
		}
		break;
	}
	if (weapons[currWeapon]->GetMesh())
		RenderHelper::RenderMesh(weapons[currWeapon]->GetMesh());
	modelStack.PopMatrix();
}

void CPlayerInfo::RenderHP()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate((float)hp / maxHp * 250 - 360, 250, 10);
	modelStack.Scale((float)hp/maxHp * 500 + 1, 20, 1);	
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("HP"));
		modelStack.PopMatrix();
}

void CPlayerInfo::RenderWeapInfo()
{			
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	modelStack.PushMatrix();
	modelStack.Translate(-350, -(float)GetCurrentWeaponClips() / GetCurrentWeaponMaxClips() * 50 + 200, 10);
	modelStack.Scale(20, (float)GetCurrentWeaponClips() / GetCurrentWeaponMaxClips() * 100 + 1, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("clip"));
	modelStack.PopMatrix();
				
	modelStack.PushMatrix();
	modelStack.Translate(-300, -(float)GetCurrentWeaponAmmo() / GetCurrentWeaponMaxAmmo() * 50 + 200, 10);
	modelStack.Scale(20, (float)GetCurrentWeaponAmmo() / GetCurrentWeaponMaxAmmo() * 100 + 1, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("mag"));
	modelStack.PopMatrix();
}

void CPlayerInfo::Move(Vector3 & position, const double dt, const Vector3 & moveBy, const float speedMultiplier)
{
	Vector3 copy = moveBy;
	copy.y = 0;
	if (!copy.IsZero())
		position += copy.Normalized() * (float)m_dSpeed * (float)dt * speedMultiplier;
}

void CPlayerInfo::UpdatePhysics(Vector3 & position, const double dt, const float speedMultiplier)
{
	Vector3 prevVel = m_vel;
	if (!IsOnGround())	
		m_vel += m_gravity * (float)m_dSpeed * (float)dt * speedMultiplier;
	position += 0.5f * (m_vel + prevVel) * (float)m_dSpeed * (float)dt * speedMultiplier;
}

void CPlayerInfo::ChangeState(const MOVEMENT_STATE& target, bool force)
{
	if (m_mState == target)
		return;
	if (!force)
	{
		switch (target)
		{		
		case MOVEMENT_STATE::MOVE:		
			if (m_mState != MOVEMENT_STATE::IDLE)
				return;
			break;
		case MOVEMENT_STATE::JUMP:
		case MOVEMENT_STATE::CROUCH:
		case MOVEMENT_STATE::PRONE:
			if (m_mState != MOVEMENT_STATE::IDLE && m_mState != MOVEMENT_STATE::MOVE && m_mState != MOVEMENT_STATE::SPRINT)
				return;
			break;
		case MOVEMENT_STATE::SPRINT:
			if (m_mState != MOVEMENT_STATE::MOVE)
				return;
			break;
		default:
		case MOVEMENT_STATE::IDLE:
		case MOVEMENT_STATE::FALL:
			//do nothing
			break;
		}
	}
	std::cout << "Changed from " << m_mState << " to " << target << "\n";
	m_mState = target;
}

void CPlayerInfo::Shoot(bool leftClick)
{
	if (leftClick)
		weapons[currWeapon]->DischargePrimary(position, target, this);
	else
		weapons[currWeapon]->DischargeSecondary(position, target, this);
}

void CPlayerInfo::TakeDamage(int amt)
{
	if (hp == 0) return;
	if (hp < 0) hp = 0;
	hp -= amt;
}

void CPlayerInfo::ResetHealth()
{
	this->hp = maxHp;
}

void CPlayerInfo::SetInertiaCam(bool b)
{
	inertiaCam = b;
}

void CPlayerInfo::UpdateState(double dt)
{
	switch (m_mState)
	{
	default:
	case MOVEMENT_STATE::IDLE:		
		SetPlayerHeight();
		sprinting = false;
		break;
	case MOVEMENT_STATE::MOVE:
		if (moveBy.IsZero())
			ChangeState(IDLE);
		SetPlayerHeight();
		sprinting = false;
		break;
	case MOVEMENT_STATE::JUMP:
		UpdatePhysics(position, dt, 5);
		if (m_vel.y < 0)
		{
			ChangeState(MOVEMENT_STATE::FALL);
		}
		break;
	case MOVEMENT_STATE::FALL:
		UpdatePhysics(position, dt, 5);
		if (position.y < m_pTerrain->GetTerrainHeight(position))
		{
			position.y = m_pTerrain->GetTerrainHeight(position);
			ChangeState(MOVEMENT_STATE::IDLE);
		}
		break;
	case MOVEMENT_STATE::CROUCH:
		SetPlayerHeight(-2);
		break;
	case MOVEMENT_STATE::PRONE:
		SetPlayerHeight(-4);
		break;
	case MOVEMENT_STATE::SPRINT:
		SetPlayerHeight();
		break;
	}
}

void CPlayerInfo::CamRotateView(Vector3 & view, const double dt, const float yaw, const float pitch, const float speedMultiplier)
{
	Mtx44 rotation;	
	float pitchCopy = pitch;
	//prevent y-overflow
	if (view.y > 0.8f && pitchCopy < 0 || view.y < -0.8f && pitchCopy > 0)
		pitchCopy = 0;
	//pitch
	rotation.SetToRotation(-pitchCopy * speedMultiplier * (float)dt, right.x, right.y, right.z);
	view = rotation * view;
	//yaw
	rotation.SetToRotation(-yaw * speedMultiplier * (float)dt, 0, 1, 0);
	view = rotation * view;	
	
	view.Normalize();	
}

void CPlayerInfo::CamRotateUp(Vector3 & up, const double dt, const float roll, const float speedMultiplier)
{
	Mtx44 rotation;	
	//roll
	rotation.SetToRotation(roll * speedMultiplier * (float)dt, view.x, 0, view.z);
	up = rotation * up;

	up.Normalize();	
}

void CPlayerInfo::UpdateTargetRightUp(Vector3 & target, Vector3 & right, Vector3 & up)
{
	target = position + view;
	right = (view.Cross(defaultUp)).Normalized();
	up = (right.Cross(view)).Normalized();
}
