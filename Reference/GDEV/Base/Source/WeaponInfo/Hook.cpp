#include "Hook.h"


CHook::CHook()
{
}


CHook::~CHook()
{
}

// Initialise this instance to default values
void CHook::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 999;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 999;
	// The current total number of rounds currently carried by this player
	totalRounds = 999;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 999;
	// The current total number of clips currently carried by this player
	totalClips = 999;
	// The max total number of clips currently carried by this player
	maxTotalClips = 999;

	// The time between shots
	timeBetweenShots = 0.3333;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	// Set the position of this weapon on the screen
	vPosition.Set(260.f, -220.f, 0.f);
	// Set the rotation of this weapon on the screen
	fRotation = -20.f;
	// Set the scale of this weapon on the screen
	vScale.Set(290.f, 200.f, 50.f);
}

void CHook::DischargePrimary(Vector3 position, Vector3 target, CPlayerInfo * _source)
{
	if (bFire)
	{
		CSoundEngine::GetInstance()->PlayASound("slap");
		// Create a projectile with a cube mesh. Its position and direction is same as the player.
		// It will last for 3.0 seconds and travel at 500 units per second
		CProjectile* aProjectile = Create::Projectile("PROJECTILE",
			position,
			(target - position).Normalized(),
			0.05f,
			500.0f,
			_source);
		aProjectile->SetCollider(true);
		aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
		bFire = false;
	}
}

void CHook::DischargeSecondary(Vector3 position, Vector3 target, CPlayerInfo * _source)
{
	if (bFire)
	{
		CSoundEngine::GetInstance()->PlayASound("woosh");		
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
		CProjectile* aProjectile = Create::Projectile("PROJECTILE",
			position,
			(target - position).Normalized(),
			2.0f,
			80.0f,
			_source);
		aProjectile->SetCollider(true);
		aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
		aProjectile->isTrigger = true;
		bFire = false;
		magRounds--;
	}
}
