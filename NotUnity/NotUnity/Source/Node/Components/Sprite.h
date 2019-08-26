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
	
	Sprite* SetHSV(float hue = -1, float sat = -1, float val = -1);
	Vector3 GetHSV();

	void ToggleCullFace(bool on);

protected:
	int currFrame;
	double currTime; 
	float frameTime;
	int selectedAnim;
	Animation* anims[13];

	Vector3 hsv;

	bool cullBackFace;
};

#endif