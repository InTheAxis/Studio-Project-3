#ifndef COLOR_SPOT_H
#define COLOR_SPOT_H

#include <string>

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../Scripts.h"
#include "../Components.h"

class ColorSpot : public Node, public TypeID<ColorSpot>, public Component
{
public:
	ColorSpot(std::string name = "ColorSpot");
	~ColorSpot();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void SetUniform(int idx);
	
	float radius;
private:
	Transform* t;	
};

#endif