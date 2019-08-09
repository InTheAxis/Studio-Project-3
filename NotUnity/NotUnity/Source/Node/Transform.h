#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Node.h"
#include "../Utility/TypeID.h"
#include "../Utility/Math/Vector3.h"
#include "../Utility/Math/Mtx44.h"
#include "../Utility/Math/MatrixStack.h"

class Transform : public Node, public TypeID<Transform>
{
public:
	Transform(std::string name = "Transform") : Node(name) 
	{
		translate.SetZero();
		rotate.SetZero();
		scale.Set(1, 1, 1);
	}
	~Transform() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	Vector3 translate;
	Vector3 rotate;
	Vector3 scale;

	Mtx44 GetModel();
};

#endif