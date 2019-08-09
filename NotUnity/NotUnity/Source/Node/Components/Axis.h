#ifndef AXIS_H
#define AXIS_H

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Math/Vector3.h"

class Axis : public Node, public TypeID<Axis>
{
public:
	Axis(std::string name = "Movement") : Node(name) {}
	~Axis() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
	void Reset();

	Vector3 RightNoY() { return Vector3(right.x, 0, right.z); }
	Vector3 DefaultUp() { return Vector3(0, 1, 0); }

	Vector3 view, right, up;
};

#endif