#pragma once

#include "Vector3.h"
#include "Mesh.h"
#include "../Projectile/Projectile.h"
#include "../SoundEngine.h"

class CPlayerInfo;

class CWeaponInfo
{
public:
	CWeaponInfo();
	virtual ~CWeaponInfo();
protected:
	// The number of ammunition in a magazine for this weapon
	int magRounds;
	// The maximum number of ammunition for this magazine for this weapon
	int maxMagRounds;
	// The current total number of rounds currently carried by this player
	int totalRounds;
	// The max total number of rounds currently carried by this player
	int maxTotalRounds;
	// The current total number of clips currently carried by this player
	int totalClips;
	// The max total number of clips currently carried by this player
	int maxTotalClips;

	// The time between shots in milliseconds
	double timeBetweenShots;
	// The elapsed time (between shots) in milliseconds
	double elapsedTime;
	// Boolean flag to indicate if weapon can fire now
	bool bFire;


	// The pointer to the mesh to be rendered
	Mesh* modelMesh;
	// Position of the weapon mesh to be rendered on the screen
	Vector3 vPosition;
	// Rotation angle of the weapon mesh to be rendered on the screen
	float fRotation;
	// Scale of the weapon mesh to be rendered on the screen
	Vector3 vScale;

public:	
	// Set the number of ammunition in the magazine for this player
	virtual void SetMagRound(const int magRounds);
	// Set the maximum number of ammunition in the magazine for this weapon
	virtual void SetMaxMagRound(const int magRounds);
	// The current total number of rounds currently carried by this player
	virtual void SetTotalRound(const int totalRounds);
	// The max total number of rounds currently carried by this player
	virtual void SetMaxTotalRound(const int maxTotalRounds);
	// The current total number of clips currently carried by this player
	virtual void SetTotalClips(const int totalClips);
	// The max total number of clips currently carried by this player
	virtual void SetMaxTotalClips(const int maxTotalClips);

	// Get the number of ammunition in the magazine for this player
	virtual int GetMagRound(void) const;
	// Get the maximum number of ammunition in the magazine for this weapon
	virtual int GetMaxMagRound(void) const;
	// Get the current total number of rounds currently carried by this player
	virtual int GetTotalRound(void) const;
	// Get the max total number of rounds currently carried by this player
	virtual int GetMaxTotalRound(void) const;
	// Get the current total number of clips currently carried by this player
	virtual int GetTotalClips(void) const;
	// Get the max total number of clips currently carried by this player
	virtual int GetMaxTotalClips(void) const;

	// Set the time between shots
	virtual void SetTimeBetweenShots(const double timeBetweenShots);
	// Set the firing rate in rounds per min
	virtual void SetFiringRate(const int firingRate);
	// Set the firing flag
	virtual void SetCanFire(const bool bFire);

	// Get the time between shots
	virtual double GetTimeBetweenShots(void) const;
	// Get the firing rate
	virtual int GetFiringRate(void) const;
	// Get the firing flag
	virtual bool GetCanFire(void) const;

	// Initialise this instance to default values
	virtual void Init(void);
	// Update the elapsed time
	void Update(const double dt);
	// Discharge this weapon
	virtual void DischargePrimary(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
	virtual void DischargeSecondary(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
	// Reload this weapon
	void Reload(void);
	// Add rounds
	void AddRounds(const int newRounds);
	// Add clip
	void AddClip(void);

	// Set the mesh to this class
	void SetMesh(Mesh* aMesh);
	// Get the mesh from this class
	Mesh* GetMesh(void) const;

	// Set the Position to this class
	void SetPosition(Vector3 vPosition);
	// Get the Position from this class
	Vector3 GetPosition(void) const;

	// Set the Rotation angle to this class
	void SetRotation(const float iRotation);
	// Get the Rotation angle from this class
	float GetRotation(void) const;

	// Set the Scale to this class
	void SetScale(Vector3 vScale);
	// Get the Scale from this class
	Vector3 GetScale(void) const;

	// Render
	virtual void Render(void);

	// Print Self
	void PrintSelf(void);

	//aim down sights
	bool ads;
};
