#include "PlayerInfo.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../Projectile/Projectile.h"
#include "../WeaponInfo/Pistol.h"
#include "MatrixStack.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "RenderHelper.h"

// Allocating and initializing CPlayerInfo's static data member.  
// The pointer is allocated but not the object's constructor.
CPlayerInfo *CPlayerInfo::s_instance = 0;

CPlayerInfo::CPlayerInfo(void)
	: attachedCamera(NULL)
	, m_pTerrain(NULL)	
	, playerHeight(0)
{	
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
void CPlayerInfo::Init(void)
{
	//set the default values
	defaultPosition.Set(0,0,10);
	defaultTarget.Set(0,0,0);
	defaultUp.Set(0,1,0);

	//set the current values
	position.Set(0, 0, 10);
	view.Set(0, 0, -1);
	UpdateTargetRightUp(target, right, up);

	//set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	//set start state
	ChangeState(m_mState, MOVEMENT_STATE::IDLE);
	playerHeight = 0;
	sprinting = false;

	//set physics var
	m_dSpeed = 1;
	m_gravity.Set(0, -9.8f, 0);
	m_vel.SetZero();

	//set the pistol as the primary weapon	
	currWeapon = 0;
	weapons[0] = new CPistol();
	weapons[0]->Init();
	weapons[1] = new CPistol();
	weapons[1]->Init();
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

// Reset this player instance to default
void CPlayerInfo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Get position x of the player
Vector3 CPlayerInfo::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CPlayerInfo::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CPlayerInfo::GetUp(void) const
{
	return up;
}

/********************************************************************************
 Hero Update
 ********************************************************************************/
void CPlayerInfo::Update(double dt)
{
	//update pos
	{
		//move wasd
		Vector3 moveBy;
		moveBy.SetZero();
		if (KeyboardController::GetInstance()->IsKeyDown('W'))
		{
			moveBy += view;
			ChangeState(m_mState, MOVEMENT_STATE::MOVE);
			if (KeyboardController::GetInstance()->IsKeyReleased(VK_LSHIFT))			
				sprinting = !sprinting;			
		}
		else if (KeyboardController::GetInstance()->IsKeyDown('S'))
		{
			moveBy += -view;
			ChangeState(m_mState, MOVEMENT_STATE::MOVE);
			sprinting = false;
		}
		if (KeyboardController::GetInstance()->IsKeyDown('A'))
		{
			if (moveBy.IsZero())
				sprinting = false;
			moveBy += -right;
			ChangeState(m_mState, MOVEMENT_STATE::MOVE);
		}		
		else if (KeyboardController::GetInstance()->IsKeyDown('D'))
		{
			if (moveBy.IsZero())
				sprinting = false;
			moveBy += right;
			ChangeState(m_mState, MOVEMENT_STATE::MOVE);
		}
		moveBy.y = 0;


		if (IsOnGround())
		{			
			//crouch and prone
			if (KeyboardController::GetInstance()->IsKeyDown(VK_LCONTROL))
			{
				ChangeState(m_mState, MOVEMENT_STATE::CROUCH);
			}
			else if (KeyboardController::GetInstance()->IsKeyDown('Z'))
			{
				ChangeState(m_mState, MOVEMENT_STATE::PRONE);
			}
			//jump and fall
			else if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE))
			{
				Vector3 temp = moveBy * 5;
				temp.y = 15;
				SetVelocity(temp);
				ChangeState(m_mState, MOVEMENT_STATE::JUMP);
			}
			else if (sprinting)
			{
				if (moveBy.IsZero())
					ChangeState(m_mState, MOVEMENT_STATE::IDLE);				
				else
					ChangeState(m_mState, MOVEMENT_STATE::SPRINT);
			}
			else if (m_mState != MOVEMENT_STATE::MOVE)
			{
				ChangeState(m_mState, MOVEMENT_STATE::IDLE);
			}
		}

		UpdateState(dt);

		//update movement based on state
		switch (m_mState)
		{
		default:
			Move(position, dt, moveBy, 40);
			break;
		case MOVEMENT_STATE::JUMP:
			Move(position, dt, moveBy, 25);
			break;
		case MOVEMENT_STATE::CROUCH:
			Move(position, dt, moveBy, 30);
			break;
		case MOVEMENT_STATE::PRONE:
			Move(position, dt, moveBy, 20);
			break;	
		case MOVEMENT_STATE::SPRINT:
			Move(position, dt, moveBy, 80);
			break;
		}
	}	
	//constrain position within boundary
	Constrain();

	//update view dir
	{
		//get mouse delta
		Vector3 mouseDelta;
		MouseController::GetInstance()->GetMouseDelta(mouseDelta.x, mouseDelta.y);
		mouseDelta.x = Math::DegreeToRadian(mouseDelta.x);
		mouseDelta.y = Math::DegreeToRadian(mouseDelta.y);

		//rotate with mouse	
		CamRotate(view, dt, mouseDelta.x, mouseDelta.y, 100);
	}
	//update target, right and up, normalized
	UpdateTargetRightUp(target, right, up);

	// If a camera is attached to this playerInfo class, then update it
	if (attachedCamera)
	{
		attachedCamera->SetCameraPos(position);
		attachedCamera->SetCameraTarget(target);
		attachedCamera->SetCameraUp(up);
	}

	// Update the weapons
	if (KeyboardController::GetInstance()->IsKeyReleased('R'))
	{
		if (weapons[currWeapon])
		{
			weapons[currWeapon]->Reload();
			weapons[currWeapon]->PrintSelf();
		}
	}
	// Press Q to add clips
	if (KeyboardController::GetInstance()->IsKeyReleased('Q'))
	{
		if (weapons[currWeapon])
			weapons[currWeapon]->AddClip();
	}
	if (weapons[currWeapon])
		weapons[currWeapon]->Update(dt);	

	currWeapon = (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) <= 0) ? 0 : 1;

	// if Mouse Buttons were activated, then act on them
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
	{
		if (weapons[currWeapon])
			weapons[currWeapon]->Discharge(position, target, this);
	}
	else if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
	{
	}

	// If the user presses P key, then reset the view to default values
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
	{
		Reset();
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
	Mesh* modelMesh = nullptr;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();

	switch (currWeapon)
	{
	case 0:
		modelMesh = MeshBuilder::GetInstance()->GetMesh("PLAYER_PISTOL");
		modelStack.Translate(250, -240, 0);
		if (!weapons[currWeapon]->GetCanFire())
		{
			modelStack.Rotate(-20.f, 0, 0, 1);
		}
		modelStack.Scale(290, 250, 1);
		break;
	case 1:
		modelMesh = MeshBuilder::GetInstance()->GetMesh("PLAYER_ASSAULT");
		modelStack.Translate(250, -240, 0);	
		if (!weapons[currWeapon]->GetCanFire())
		{
			modelStack.Rotate(-20.f, 0, 0, 1);
		}
		modelStack.Scale(400, 250, 1);
		break;
	}
	if (modelMesh)
		RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

void CPlayerInfo::Move(Vector3 & position, const double dt, const Vector3 & moveBy, const float speedMultiplier)
{
	if (!moveBy.IsZero())
		position += moveBy.Normalized() * (float)m_dSpeed * (float)dt * speedMultiplier;
}

void CPlayerInfo::UpdatePhysics(Vector3 & position, const double dt, const float speedMultiplier)
{
	Vector3 prevVel = m_vel;
	if (!IsOnGround())	
		m_vel += m_gravity * (float)m_dSpeed * (float)dt * speedMultiplier;
	position += 0.5f * (m_vel + prevVel) * (float)m_dSpeed * (float)dt * speedMultiplier;
}

void CPlayerInfo::ChangeState(MOVEMENT_STATE& mState, const MOVEMENT_STATE& target, bool force)
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

void CPlayerInfo::UpdateState(double dt)
{
	switch (m_mState)
	{
	default:
	case MOVEMENT_STATE::IDLE:
	case MOVEMENT_STATE::MOVE:
		SetPlayerHeight();
		sprinting = false;
		break;
	case MOVEMENT_STATE::JUMP:
		UpdatePhysics(position, dt, 5);
		if (m_vel.y < 0)
		{
			ChangeState(m_mState, MOVEMENT_STATE::FALL);
		}
		break;
	case MOVEMENT_STATE::FALL:
		UpdatePhysics(position, dt, 5);
		if (position.y < m_pTerrain->GetTerrainHeight(position))
		{
			position.y = m_pTerrain->GetTerrainHeight(position);
			ChangeState(m_mState, MOVEMENT_STATE::IDLE);
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

void CPlayerInfo::CamRotate(Vector3 & view, const double dt, const float yaw, const float pitch, const float speedMultiplier)
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
	rotation.SetToRotation(-yaw * speedMultiplier * (float)dt, up.x, up.y, up.z);
	view = rotation * view;	
	
	view.Normalize();	
}

void CPlayerInfo::UpdateTargetRightUp(Vector3 & target, Vector3 & right, Vector3 & up)
{
	target = position + view;
	right = (view.Cross(defaultUp)).Normalized();
	up = (right.Cross(view)).Normalized();
}
