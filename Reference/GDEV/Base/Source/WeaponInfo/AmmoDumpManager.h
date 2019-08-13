#pragma once

#include "SingletonTemplate.h"
#include <map>
#include "AmmoDump.h"

class CAmmoDumpManager : public Singleton<CAmmoDumpManager>
{
public:
	CAmmoDumpManager();
	virtual ~CAmmoDumpManager();

	// Init this instance
	void Init(void);
	// Destroy this instance
	void Destroy(void);

	// Add an ammo dump to the list
	void Add(Vector3 vPosition);
	void Remove(int key);

	// Check Proximity of a position to any ammo dump stored here
	int CheckProximity(Vector3 vPosition, const float fDistance = 28.28f);

	// Get the number of Ammo dumps in this map
	int GetNumOfAmmoDumps(void) const;

	// Render the Ammo Dumps
	void Render(void);

protected:

	// The map of all the ammo dumps in this CAmmoDumpManager
	std::map<int, CAmmoDump*> mpAmmoDump;
};