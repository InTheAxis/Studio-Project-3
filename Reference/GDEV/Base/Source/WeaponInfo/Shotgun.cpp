#include "Shotgun.h"

#include "Mtx44.h"

CShotgun::CShotgun()
{
}


CShotgun::~CShotgun()
{
}

// Initialise this instance to default values
void CShotgun::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 20;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 20;
	// The current total number of rounds currently carried by this player
	totalRounds = 240;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 240;
	// The current total number of clips currently carried by this player
	totalClips = 1;
	// The max total number of clips currently carried by this player
	maxTotalClips = 5;

	// The time between shots
	timeBetweenShots = 1.0f;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	// Set the position of this weapon on the screen
	vPosition.Set(200.f, -220.f, 0.f);
	// Set the rotation of this weapon on the screen
	fRotation = -10.f;
	// Set the scale of this weapon on the screen
	vScale.Set(400.f, 250.f, 1.f);
}

void CShotgun::DischargePrimary(Vector3 position, Vector3 target, CPlayerInfo * _source)
{	
	float spread = 0.1f;
	if (bFire)
	{
		if (magRounds > 0)
		{
			CSoundEngine::GetInstance()->PlayASound("shotgun");
			// If there is still ammo in the magazine, then fire
			for (int i = 0; i < 5; ++i)
			{
				if (magRounds > 0)
				{
					CSoundEngine::GetInstance()->PlayASound("shotgun");
					Vector3 view = (target - position);
					view = view + Vector3(Math::RandFloatMinMax(-spread, spread), Math::RandFloatMinMax(-spread, spread), 0);
					view.Normalize();
					// Create a projectile with a cube mesh. Its position and direction is same as the player.
					// It will last for 3.0 seconds and travel at 500 units per second
					CProjectile* aProjectile = Create::Projectile("PROJECTILE",
						position + view,
						view,
						0.3f,
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

void CShotgun::DischargeSecondary(Vector3 position, Vector3 target, CPlayerInfo * _source)
{
	ads = !ads;
}
