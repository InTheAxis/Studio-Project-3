#ifndef COMP_KINEMATICBODY_H
#define COMP_KINEMATICBODY_H

#include "ComponentBase.h"
#include "Vector3.h"

class CompKinematicBody : public Component<CompKinematicBody>
{
public:
	CompKinematicBody()
		: impulse(0, 0, 0)
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
		, gravScale(1)
		, cog(0,0,0)
		, rad(0)
	{}
	~CompKinematicBody() {}

	Vector3 impulse;
	float torque;
	float mass, momentOfInertia;	
	Vector3 prevLinearVel, linearVel;	
	float prevAngVel, angVel;
	float gravScale;
	float rad;
	Vector3 cog;

	//constants to be set later
	float FORCE_MAG, TORQUE_MAG;
	float MAX_LIN_VEL, MAX_ANG_VEL;	
};

#endif