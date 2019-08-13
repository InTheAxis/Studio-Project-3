#ifndef SPRITE_H
#define SPRITE_H

#include "Renderable.h"
#include "../../Utility/Graphics/Animation.h"

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

	Sprite* SetAnimation(int idx, int numFrames, float animTime, bool repeat);	
	Sprite* SwitchAnimation(int idx);
	Sprite* PlayAnimation();

protected:
	int currFrame;
	double currTime; 
	float frameTime;

	int selectedAnim;

	Animation* anims[8];
};

#endif