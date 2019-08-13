#include "AssaultRifle.h"


CAssaultRifle::CAssaultRifle()
{
}


CAssaultRifle::~CAssaultRifle()
{
}

// Initialise this instance to default values
void CAssaultRifle::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 30;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 30;
	// The current total number of rounds currently carried by this player
	totalRounds = 240;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 240;
	// The current total number of clips currently carried by this player
	totalClips = 1;
	// The max total number of clips currently carried by this player
	maxTotalClips = 5;

	// The time between shots
	timeBetweenShots = 0.3333;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	// Set the position of this weapon on the screen
	vPosition.Set(180.f, -220.f, 0.f);
	// Set the rotation of this weapon on the screen
	fRotation = -10.f;
	// Set the scale of this weapon on the screen
	vScale.Set(400.f, 250.f, 1.f);	
}

void CAssaultRifle::DischargePrimary(Vector3 position, Vector3 target, CPlayerInfo * _source)
{	
	if (bFire)
	{
		if (magRounds > 0)
		{
			CSoundEngine::GetInstance()->PlayASound("pistol");
			for (int i = 1; i <= 3; ++i)
			{
				float spread = 0.01f;
				// If there is still ammo in the magazine, then fire
				if (magRounds > 0)
				{
					Vector3 view = (target - position);
					view += Vector3(Math::RandFloatMinMax(-spread, spread), Math::RandFloatMinMax(-spread, spread), 0);
					view.Normalize();
					// Create a projectile with a cube mesh. Its position and direction is same as the player.
					// It will last for 3.0 seconds and travel at 500 units per second
					CProjectile* aProjectile = Create::Projectile("PROJECTILE",
						position + view * i * 10,
						view,
						2.0f,
						100.0f,
						_source);
					aProjectile->SetCollider(true);
					aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
					bFire = false;
					magRounds--;
				}
			}
		}
		else
		{
			CSoundEngine::GetInstance()->PlayASound("noammo");
		}		
	}
}

void CAssaultRifle::DischargeSecondary(Vector3 position, Vector3 target, CPlayerInfo * _source)
{	
	ads = !ads;
}
