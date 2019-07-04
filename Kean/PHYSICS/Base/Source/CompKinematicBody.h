#ifndef COMP_KINEMATICBODY_H
#define COMP_KINEMATICBODY_H

#include "ComponentBase.h"
#include "Vector3.h"

class CompKinematicBody : public Component<CompKinematicBody>
{
public:
	CompKinematicBody() 
		: force(0, 0, 0)
		, torque(0)
		, mass(1.f)
		, momentOfInertia(1.f)
		, linearVel(0, 0, 0)
		, prevLinearVel(0, 0, 0)
		, angVel(0)
		, prevAngVel(0)
		, FORCE_MAG(0)
		, TORQUE_MAG(0)
		, MAX_LIN_VEL(1)
		, MAX_ANG_VEL(1)
	{}
	~CompKinematicBody() {}

	Vector3 force;
	float torque;
	float mass, momentOfInertia;	
	Vector3 prevLinearVel, linearVel;	
	float prevAngVel, angVel;

	//constants to be set later
	float FORCE_MAG, TORQUE_MAG;
	float MAX_LIN_VEL, MAX_ANG_VEL;
};

#endif