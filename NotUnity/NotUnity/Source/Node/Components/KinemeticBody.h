#ifndef KINEMETICBODY_H
#define KINEMETICBODY_H

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../../Utility/Math/Vector3.h"

class KinemeticBody : public Node, public TypeID<KinemeticBody>, public Component
{
public:
	KinemeticBody(std::string name = "KinemeticBody");
	~KinemeticBody();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
	
	
	
	float mass;
	Vector3 maxVel;
	float frictionCoeff;
	bool useGravity;
	float gravScale;

	void ApplyForce(Vector3 force);
	void ApplyImpulse(Vector3 impulse);

	void UpdateSuvat(double dt);

	void ResetForce();
	void ResetVel(bool x , bool y, bool z = true);
	Vector3 GetVel() const;

private:
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 gravity;
	Vector3 force;
	Vector3 impulse;
};

#endif