#ifndef COMP_TRACKER_H
#define COMP_TRACKER_H

#include "ComponentBase.h"

class GameObject;
class CompTracker : public Component<CompTracker>
{
public:
	CompTracker()
		: target(nullptr)
	{}
	~CompTracker() {}

	GameObject* target;
};

#endif