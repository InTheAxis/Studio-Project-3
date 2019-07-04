#ifndef COMP_TRANSFORM_H
#define COMP_TRANSFORM_H

#include "ComponentBase.h"
#include "Vector3.h"

class CompTransform : public Component<CompTransform>
{
public:
	CompTransform() 
		: translate(0, 0, 0)
		, scale(1, 1, 1)		
		, dir(0, 1, 0)
	{}
	~CompTransform() {}

	Vector3 translate, scale;
	Vector3 dir;
};

#endif