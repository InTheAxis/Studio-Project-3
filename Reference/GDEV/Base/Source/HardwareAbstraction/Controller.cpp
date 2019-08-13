#include "Controller.h"
#include <iostream>
using namespace std;

CController::CController()
	: thePlayerInfo(NULL)
{
}


CController::~CController()
{
	// We just set thePlayerInfo to NULL without deleting. SceneText will delete this.
	if (thePlayerInfo)
		thePlayerInfo = NULL;
}


// Create this controller
bool CController::Create(CPlayerInfo* thePlayerInfo)
{	
	this->thePlayerInfo = thePlayerInfo;
	return false;
}

// Read from the controller
int CController::Read(const const float deltaTime)
{
	//cout << "CController::Read()" << endl;
	return 0;
}

// Detect and process front / back movement on the controller
bool CController::Move_FrontBack(const int direction)
{	
	if (thePlayerInfo)
	{		
		thePlayerInfo->AddToMoveBy(direction * thePlayerInfo->GetView());
		thePlayerInfo->ChangeState(CPlayerInfo::MOVE);			
		if (direction < 0)
			thePlayerInfo->ToggleSprinting(false);
		return true;
	}
	return false;
}

// Detect and process left / right movement on the controller
bool CController::Move_LeftRight(const int direction)
{
	if (thePlayerInfo)
	{
		thePlayerInfo->AddToMoveBy(direction * thePlayerInfo->GetRight());
		thePlayerInfo->ChangeState(CPlayerInfo::MOVE);		
		return true;
	}
	return false;
}

void CController::Sprint()
{
	if (thePlayerInfo && thePlayerInfo->IsOnGround())
		thePlayerInfo->ToggleSprinting();
}

void CController::Crouch()
{
	if (!thePlayerInfo || !thePlayerInfo->IsOnGround())
		return;
	thePlayerInfo->ChangeState(CPlayerInfo::CROUCH);
	thePlayerInfo->ToggleSprinting(false);
}

void CController::Prone()
{
	if (!thePlayerInfo || !thePlayerInfo->IsOnGround())
		return;
	thePlayerInfo->ChangeState(CPlayerInfo::PRONE);
	thePlayerInfo->ToggleSprinting(false);

}

void CController::Jump()
{
	if (!thePlayerInfo || !thePlayerInfo->IsOnGround())
		return;
	Vector3 temp = thePlayerInfo->GetMoveBy() * 5;
	temp.y = 15;
	thePlayerInfo->SetVelocity(temp);
	thePlayerInfo->ChangeState(CPlayerInfo::JUMP);
}

void CController::Idle()
{
	if (!thePlayerInfo || !thePlayerInfo->IsOnGround())
		return;
	if (thePlayerInfo->GetSprinting())
	{
		if (thePlayerInfo->GetMoveBy().IsZero())
		{
			thePlayerInfo->ChangeState(CPlayerInfo::IDLE);
			thePlayerInfo->ToggleSprinting(false);
		}
		else
			thePlayerInfo->ChangeState(CPlayerInfo::SPRINT);
	}
	else if (thePlayerInfo->GetState() != CPlayerInfo::MOVE)
			thePlayerInfo->ChangeState(CPlayerInfo::IDLE);
}

bool CController::UpdateMouseDelta(float xDeg, float yDeg)
{
	if (thePlayerInfo)
	{
		thePlayerInfo->SetMouseDelta(Vector3(Math::DegreeToRadian(xDeg), Math::DegreeToRadian(yDeg), 0));		
		return true;
	}
	return false;
}

bool CController::UpdateArrowKeyDelta(float xDeg, float yDeg)
{
	if (thePlayerInfo)
	{
		thePlayerInfo->SetArrowKeyDelta(Vector3(Math::DegreeToRadian(xDeg), Math::DegreeToRadian(yDeg), 0));
		return true;
	}
	return false;
}

bool CController::Reload()
{
	if (thePlayerInfo)
	{
		thePlayerInfo->GetWeapon()->Reload();
		thePlayerInfo->GetWeapon()->PrintSelf();
		return true;
	}
	return false;
}

bool CController::AmmoPickUp()
{
	if (thePlayerInfo)
	{
		int temp = CAmmoDumpManager::GetInstance()->CheckProximity(thePlayerInfo->GetPos());
		if (temp >= 0)
		{
			thePlayerInfo->GetWeapon()->AddClip();
			CAmmoDumpManager::GetInstance()->Remove(temp);
			thePlayerInfo->GetWeapon()->PrintSelf();
		}
		return true;
	}
	return false;
}

bool CController::Change(int scrollStat)
{
	if (thePlayerInfo)
	{
		thePlayerInfo->ChangeWeapon(scrollStat <= 0 ? 0 : 1);
		return true;
	}
	return false;
}

bool CController::FirePrimary()
{	
	if (thePlayerInfo)
	{
		thePlayerInfo->Shoot(true);
		return true;
	}
	return false;
}

bool CController::FireSecondary()
{	
	if (thePlayerInfo)
	{
		thePlayerInfo->Shoot(false);
		return true;
	}
	return false;
}