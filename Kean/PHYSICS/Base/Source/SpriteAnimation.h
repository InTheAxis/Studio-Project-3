#ifndef SPRITE_ANIM_H
#define SPRITE_ANIM_H

#include "CompMesh.h"
#include "MyMath.h"

struct Animation 
{
	Animation() {}

	void Set(int startFrame, int endFrame, int repeat, float time, bool active)
	{		
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->repeatCount = repeat;
		this->animTime = time;
		this->animActive = active;
	}

	int startFrame;
	int endFrame;
	int repeatCount;
	float animTime;
	bool ended;
	bool animActive;
};

class SpriteAnimation : public CompMesh
{
public:
	SpriteAnimation();
	SpriteAnimation(int row, int col);
	~SpriteAnimation();
	void Update(double dt);
	//virtual void Draw(); // Polymorphism
	int m_row;
	int m_col;
	float m_currentTime;
	int m_currentFrame;
	int m_playCount;
	Animation *m_anim;
};

#endif
