#ifndef ANIMATION_H
#define ANIMATION_H

class Animation
{
public:
	Animation(int startFrame = 0, int endFrame = 0, bool repeat = true, double animTime = 1.f, bool ended = false)
		: startFrame(startFrame)
		, endFrame(endFrame)
		, repeat(repeat)
		, animTime(animTime)
		, play(false)
	{
	}

	~Animation()
	{
	}

	int startFrame;
	int endFrame;
	bool repeat;
	double animTime;
	bool play;
};

#endif