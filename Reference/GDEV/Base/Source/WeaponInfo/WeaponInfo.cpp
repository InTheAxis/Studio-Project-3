#include "WeaponInfo.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"

#include <iostream>
using namespace std;

CWeaponInfo::CWeaponInfo()
	: magRounds(1)
	, maxMagRounds(1)
	, totalRounds(8)
	, maxTotalRounds(8)
	, totalClips(8)
	, maxTotalClips(8)
	, timeBetweenShots(0.5)
	, elapsedTime(0.0)
	, bFire(true)
	, ads(false)
	, modelMesh(NULL)
{
}

// Destructor
CWeaponInfo::~CWeaponInfo()
{
	// Set the modelMesh to NULL but let MeshBuilder delete the instance
	if (modelMesh)
		modelMesh = NULL;
}

// Set the number of ammunition in the magazine for this player
void CWeaponInfo::SetMagRound(const int magRounds)
{
	this->magRounds = magRounds;
}

// Set the maximum number of ammunition in the magazine for this weapon
void CWeaponInfo::SetMaxMagRound(const int magRounds)
{
	this->magRounds = magRounds;
}

// The current total number of rounds currently carried by this player
void CWeaponInfo::SetTotalRound(const int totalRounds)
{
	this->totalRounds = totalRounds;
}

// The max total number of rounds currently carried by this player
void CWeaponInfo::SetMaxTotalRound(const int maxTotalRounds)
{
	this->maxTotalRounds = maxTotalRounds;
}

// The current total number of clips currently carried by this player
void CWeaponInfo::SetTotalClips(const int totalClips)
{
	this->totalClips = totalClips;
}
// The max total number of clips currently carried by this player
void CWeaponInfo::SetMaxTotalClips(const int maxTotalClips)
{
	this->maxTotalClips = maxTotalClips;
}

// Get the number of ammunition in the magazine for this player
int CWeaponInfo::GetMagRound(void) const
{
	return magRounds;
}

// Get the maximum number of ammunition in the magazine for this weapon
int CWeaponInfo::GetMaxMagRound(void) const
{
	return maxMagRounds;
}

// Get the current total number of rounds currently carried by this player
int CWeaponInfo::GetTotalRound(void) const
{
	return totalRounds;
}

// Get the max total number of rounds currently carried by this player
int CWeaponInfo::GetMaxTotalRound(void) const
{
	return maxTotalRounds;
}

// Get the current total number of clips currently carried by this player
int CWeaponInfo::GetTotalClips(void) const
{
	return totalClips;
}
// Get the max total number of clips currently carried by this player
int CWeaponInfo::GetMaxTotalClips(void) const
{
	return maxTotalClips;
}

// Set the time between shots
void CWeaponInfo::SetTimeBetweenShots(const double timeBetweenShots)
{
	this->timeBetweenShots = timeBetweenShots;
}

// Set the firing rate in rounds per min
void CWeaponInfo::SetFiringRate(const int firingRate)
{
	timeBetweenShots = 60.0 / (double)firingRate;	// 60 seconds divided by firing rate
}

// Set the firing flag
void CWeaponInfo::SetCanFire(const bool bFire)
{
	this->bFire = bFire;
}

// Get the time between shots
double CWeaponInfo::GetTimeBetweenShots(void) const
{
	return timeBetweenShots;
}

// Get the firing rate
int CWeaponInfo::GetFiringRate(void) const
{
	return (int)(60.0 / timeBetweenShots);	// 60 seconds divided by timeBetweenShots
}

// Get the firing flag
bool CWeaponInfo::GetCanFire(void) const
{
	return bFire;
}

// Initialise this instance to default values
void CWeaponInfo::Init(void)
{
	// The number of ammunition in a magazine for this weapon
	magRounds = 1;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 1;
	// The current total number of rounds currently carried by this player
	totalRounds = 8;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 8;
	// The current total number of clips currently carried by this player
	totalClips = 1;
	// The max total number of clips currently carried by this player
	maxTotalClips = 1;

	// The time between shots
	timeBetweenShots = 0.5;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

// Update the elapsed time
void CWeaponInfo::Update(const double dt)
{
	elapsedTime += dt;
	if (elapsedTime > timeBetweenShots)
	{
		bFire = true;
		elapsedTime = 0.0;
	}
}

// Discharge this weapon
void CWeaponInfo::DischargePrimary(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			CProjectile* aProjectile = Create::Projectile("PROJECTILE", 
															position, 
															(target - position).Normalized(), 
															2.0f, 
															10.0f,
															_source);
			aProjectile->SetCollider(true);
			aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;
		}
		CSoundEngine::GetInstance()->PlayASound("noammo");
	}
	CSoundEngine::GetInstance()->PlayASound("noammo");
}

void CWeaponInfo::DischargeSecondary(Vector3 position, Vector3 target, CPlayerInfo * _source)
{
	CSoundEngine::GetInstance()->PlayASound("noammo");
}

// Reload this weapon
void CWeaponInfo::Reload(void)
{	
	// This version is for using rounds, without clips
	//if (magRounds < maxMagRounds)
	//{
	//	if (maxMagRounds - magRounds <= totalRounds)
	//	{
	//		totalRounds -= maxMagRounds - magRounds;
	//		magRounds = maxMagRounds;
	//	}
	//	else
	//	{
	//		magRounds += totalRounds;
	//		totalRounds = 0;
	//	}
	//}

	// This version is for using clips
	// if the current rounds in weapon is less than the max, then allow reload
	if (magRounds < maxMagRounds)
	{
		if (totalClips > 0)
		{
			magRounds = maxMagRounds;	// Set the current round in weapon to max
			totalClips--;
			CSoundEngine::GetInstance()->PlayASound("reload");
		}
	}
}

// Add rounds
void CWeaponInfo::AddRounds(const int newRounds)
{
	if (totalRounds + newRounds > maxTotalRounds)
		totalRounds = maxTotalRounds;
	else
		totalRounds += newRounds;
}

// Add clip
void CWeaponInfo::AddClip(void)
{
	// This version is for using clips
	if (totalClips < maxTotalClips)
	{
		if (magRounds != maxMagRounds)
			magRounds = maxMagRounds;
		else
			totalClips++;
	}
}

// Set the mesh to this class
void CWeaponInfo::SetMesh(Mesh* aMesh)
{
	this->modelMesh = aMesh;
}

// Get the mesh from this class
Mesh* CWeaponInfo::GetMesh(void) const
{
	return modelMesh;
}

// Set the Position to this class
void CWeaponInfo::SetPosition(Vector3 vPosition)
{
	this->vPosition = vPosition;
}

// Get the Position from this class
Vector3 CWeaponInfo::GetPosition(void) const
{
	return vPosition;
}

// Set the Rotation angle to this class
void CWeaponInfo::SetRotation(const float fRotation)
{
	this->fRotation = fRotation;
}

// Get the Rotation angle from this class
float CWeaponInfo::GetRotation(void) const
{
	return fRotation;
}

// Set the Scale to this class
void CWeaponInfo::SetScale(Vector3 vScale)
{
	this->vScale = vScale;
}

// Get the Scale from this class
Vector3 CWeaponInfo::GetScale(void) const
{
	return vScale;
}

// Render
void CWeaponInfo::Render(void)
{
	if (modelMesh == NULL)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
		//modelMesh = MeshBuilder::GetInstance()->GetMesh("PLAYER_PISTOL");
		// Translate weapon mesh to be rendered on the screen
		modelStack.Translate(vPosition.x, vPosition.y, vPosition.z);
		if (!GetCanFire())
		{
			// Rotate weapon mesh to be rendered on the screen
			modelStack.Rotate(fRotation, 0.f, 0.f, 1.f);
		}
		modelStack.Scale(vScale.x, vScale.y, vScale.z);
		RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

// Print Self
void CWeaponInfo::PrintSelf(void)
{
	cout << "CWeaponInfo::PrintSelf()" << endl;
	cout << "========================" << endl;
	cout << "magRounds\t\t:\t" << magRounds << endl;
	cout << "maxMagRounds\t\t:\t" << maxMagRounds << endl;
	cout << "totalRounds\t\t:\t" << totalRounds << endl;
	cout << "maxTotalRounds\t\t:\t" << maxTotalRounds << endl;
	cout << "totalClips\t\t:\t" << totalClips << endl;
	cout << "maxTotalClips\t\t:\t" << maxTotalClips << endl;
	cout << "timeBetweenShots\t:\t" << timeBetweenShots << endl;
	cout << "elapsedTime\t\t:\t" << elapsedTime << endl;
	cout << "bFire\t\t:\t" << bFire << endl;
}
