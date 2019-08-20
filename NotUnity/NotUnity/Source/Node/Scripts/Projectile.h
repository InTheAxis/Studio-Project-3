#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <string>

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../../Utility/Math/Vector3.h"
#include "../Scripts.h"
#include "../Components.h"

class KinemeticBody;
class Projectile : public Node, public TypeID<Projectile>, public Component
{
public:
	Projectile(std::string name = "Projectile");
	~Projectile();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void Discharge(Vector3 origin, Vector3 vel);

private:
	Transform* t;	
	KinemeticBody* kinb;
};

#endif