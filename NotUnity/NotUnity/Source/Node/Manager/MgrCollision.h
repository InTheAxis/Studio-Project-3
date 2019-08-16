#ifndef MGR_COLLISION_H
#define MGR_COLLISION_H

#include "Manager.h"

#include <vector>

class Collider;
class MgrCollision : public Manager<MgrCollision>
{
	friend Singleton<MgrCollision>;
	friend Node;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void RegisterColl(Collider* c);
	void UnregisterColl(Collider* c);

protected:
	MgrCollision(std::string name = "MgrCollision");
	~MgrCollision();

	std::vector<Collider*> allColls;
};
#endif