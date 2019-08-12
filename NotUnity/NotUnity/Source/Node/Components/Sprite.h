#ifndef SPRITE_H
#define SPRITE_H

#include "../../Utility/TypeID.h"
#include "Renderable.h"

class MgrGraphics;
class Sprite : public Renderable
{
public:
	Sprite(std::string name = "Sprite");
	~Sprite();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	virtual void Render();

protected:

};

#endif