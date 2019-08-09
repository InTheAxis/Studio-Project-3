#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "Node.h"
#include "../Utility/TypeID.h"

class Components : public Node, public TypeID<Components>
{
public:
	Components(std::string name = "Components") : Node(name) {}
	~Components() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	enum PRESET
	{
		JSON = -1,
		EMPTY = 0,
		CAMERA_DEBUG,
		RENDERABLE,
	};

	void LoadPreset(Components::PRESET p = EMPTY);
};

#endif