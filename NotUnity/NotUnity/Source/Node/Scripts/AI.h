#ifndef AI_H
#define AI_H

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Scripts.h"
#include "../Components.h"
#include "../Components/Transform.h"

class AI : public Node, public TypeID<AI>, public Component
{
public:
	AI(std::string name = "AI");
	~AI();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	void setHealth(int health);

private:
	int health;
	int damage;
};

#endif