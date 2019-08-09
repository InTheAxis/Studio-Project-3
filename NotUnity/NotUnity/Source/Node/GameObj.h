#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include "Node.h"
#include "../Utility/TypeID.h"
#include "Components.h"
#include "Scripts.h"

class Transform;
class GameObj : public Node, public TypeID<GameObj>
{
public:
	GameObj(std::string name = "GameObj") : Node(name) {}
	~GameObj() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	bool Create(std::string filePath);
	bool Create(Components::PRESET cPreset = Components::EMPTY, Scripts::PRESET sPreset = Scripts::EMPTY);

private:
	//refrences to children
	Components* comps;
	Scripts* scripts;
	Transform* t;
};

#endif