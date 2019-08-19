#ifndef COLLIDER_H
#define COLLIDER_H

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
	Collider* other;
	Vector3 penetration;
};

class Transform;
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

	Delegate<ColInfo> OnCollide; //pass in other collider
	Delegate<ColInfo> OnTrigger; //pass in other collider

	bool isTrigger;

private:
	Transform* t;
	AABB aabb;
};

#endif