#ifndef COLLIDER_H
#define COLLIDER_H

#include <string>

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../../Utility/Delegate.h"
#include "../../Utility/Math/AABB.h"
#include "../../Utility/Math/Vector3.h"

class Collider;
class ColInfo
{
public:
	Collider* coll;
	Collider* other;
	Vector3 penetration;
};

class Transform;
class ColliderRender;
class Collider : public Node, public TypeID<Collider>, public Component
{
public:
	Collider(std::string name = "Collider");
	~Collider();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	Collider* CreateAABB(float radius);
	Collider* CreateAABB(Vector3 min, Vector3 max);

	void CheckCollision(Collider* coll);

	Delegate<ColInfo> OnCollideEnter; //pass in other collider
	Delegate<ColInfo> OnTriggerEnter; //pass in other collider
	Delegate<ColInfo> OnCollideStay; //pass in other collider
	Delegate<ColInfo> OnTriggerStay; //pass in other collider
	Delegate<ColInfo> OnCollideExit; //pass in other collider
	Delegate<ColInfo> OnTriggerExit; //pass in other collider

	bool isTrigger;

	std::string tag;

private:
	Transform* t;
	AABB aabb;
	ColliderRender* cr;

	bool collided;
	bool triggered;
};

#endif