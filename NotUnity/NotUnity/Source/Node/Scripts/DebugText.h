#ifndef DEBUGTEXT_H
#define DEBUGTEXT_H

#include <sstream>

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../Scripts.h"
#include "../Components/Text.h"
#include "../Components.h"

class DebugText : public Node, public TypeID<DebugText>
{
public:
	DebugText(std::string name = "DebugText"); 
	~DebugText();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
private:
	std::stringstream ss;
	Text* fps;
};

#endif