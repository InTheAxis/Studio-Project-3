#ifndef KINEMATIC_BODY_H
#define KINEMATIC_BODY_H

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../../Utility/Math/Vector3.h"

class KinematicBody : public Node, public TypeID<KinematicBody>, public Component
{
public:
	KinematicBody(std::string name = "KinemeticBody");
	~KinematicBody();
	
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
};

#endif