#pragma once

#include "WeaponInfo.h"

class CShotgun:
	public CWeaponInfo
{
public:
	CShotgun();
	virtual ~CShotgun();

	// Initialise this instance to default values
	void Init(void);
	void DischargePrimary(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
	void DischargeSecondary(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
};

