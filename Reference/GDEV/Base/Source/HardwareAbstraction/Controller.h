#pragma once

#include "../../Base/Source/PlayerInfo/PlayerInfo.h"
#include "../../Base/Source/WeaponInfo/AmmoDumpManager.h"

class CController
{
protected:
	CPlayerInfo* thePlayerInfo;

public:
	CController();
	virtual ~CController();
	
	virtual bool Create(CPlayerInfo* thePlayerInfo = NULL);
	virtual int Read(const float deltaTime);
	
	//player movement

	bool Move_FrontBack(const int direction);
	bool Move_LeftRight(const int direction);
	void Sprint();
	void Crouch();
	void Prone();
	void Jump();
	void Idle();
	
	bool UpdateMouseDelta(float xDeg, float yDeg);	
	bool UpdateArrowKeyDelta(float xDeg, float yDeg);	
	
	//weapons

	bool Reload();
	bool AmmoPickUp();
	bool Change(int scrollStat);	
	bool FirePrimary();	
	bool FireSecondary();	
};

