#ifndef COMP_SPRITE_ANIM_H
#define COMP_SPRITE_ANIM_H

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

class CompSpriteAnim : public CompMesh
{
public:
	CompSpriteAnim();
	CompSpriteAnim(int row, int col);
	~CompSpriteAnim();
	void Update(double dt);	
	int m_row;
	int m_col;
	float m_currentTime;
	int m_currentFrame;
	int m_playCount;
	Animation *m_anim;
};

#endif
