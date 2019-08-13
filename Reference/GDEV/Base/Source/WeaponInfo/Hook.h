#pragma once

#include "WeaponInfo.h"

class CHook :
	public CWeaponInfo
{
public:
	CHook();
	virtual ~CHook();

	// Initialise this instance to default values
	void Init(void);
	void DischargePrimary(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
	void DischargeSecondary(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
};

