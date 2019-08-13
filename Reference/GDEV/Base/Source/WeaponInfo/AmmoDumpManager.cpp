#include "AmmoDumpManager.h"

#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"

CAmmoDumpManager::CAmmoDumpManager()
{
}


CAmmoDumpManager::~CAmmoDumpManager()
{
	// Call the Destroy() in this class
	this->Destroy();
}

// Init this instance
void CAmmoDumpManager::Init(void)
{
	// We provide a default position as a demo
	Add(Vector3(0.0f, -10.0f, -20.0f));
	Add(Vector3(40.0f, -10.0f, -20.0f));
	Add(Vector3(-40.0f, -10.0f, -20.0f));
}

// Destroy this instance
void CAmmoDumpManager::Destroy(void)
{
	// Delete all AmmoDump instances
	for (std::map<int, CAmmoDump*>::iterator itr = mpAmmoDump.begin(); itr != mpAmmoDump.end(); itr++)
	{
		delete (itr->second);
	}
	mpAmmoDump.clear();
}

// Add an ammo dump to the list
void CAmmoDumpManager::Add(Vector3 vPosition)
{
	// Create a new Ammo Dump instance
	CAmmoDump* cAmmoDump = new CAmmoDump();
	// Set a position to this ammo dump
	cAmmoDump->SetPosition(vPosition);

	// Add to AmmoDump map
	mpAmmoDump.insert({ GetNumOfAmmoDumps(), cAmmoDump });
}

// Add an ammo dump to the list
void CAmmoDumpManager::Remove(int key)
{
	if (mpAmmoDump.count(key) <= 0)
		return;
	mpAmmoDump.erase(key);
}

// Check Proximity of a position to any ammo dump stored here
int CAmmoDumpManager::CheckProximity(Vector3 vPosition, const float fDistance)
{
	for (std::map<int, CAmmoDump*>::iterator itr = mpAmmoDump.begin(); itr != mpAmmoDump.end(); itr++)
	{
		// Get the position of an Ammo Dump
		Vector3 vAmmoDumpPosition = itr->second->GetPosition();

		Vector3 vDistance = vPosition - vAmmoDumpPosition;
		vDistance.y = 0.0f;
		if (vDistance.LengthSquared() < fDistance * fDistance)
			return itr->first;
	}

	return -999;
}

// Get the number of Ammo dumps in this map
int CAmmoDumpManager::GetNumOfAmmoDumps(void) const
{
	return mpAmmoDump.size();
}

// Render this AmmoDump
void CAmmoDumpManager::Render(void)
{
	for (std::map<int, CAmmoDump*>::iterator itr = mpAmmoDump.begin(); itr != mpAmmoDump.end(); itr++)
	{
		itr->second->Render();
	}
}