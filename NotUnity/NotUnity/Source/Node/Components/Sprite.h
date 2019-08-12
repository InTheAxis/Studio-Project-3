#ifndef SPRITE_H
#define SPRITE_H

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

	void SwitchAnimation(int idx);

protected:
	int currFrame;
	double currTime;
	int startFrame;
	int endFrame;
	bool repeat;
	double animTime;
	bool ended;

	float frameTime;

	int selectedAnim;
};

#endif