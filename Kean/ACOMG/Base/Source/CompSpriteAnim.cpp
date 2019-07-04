#include "CompSpriteAnim.h"

#include "GL\glew.h"
#include "Vertex.h"

CompSpriteAnim::CompSpriteAnim(int row, int col)
	: m_row(row)
	, m_col(col)
	, m_currentTime(0)
	, m_currentFrame(0)
	, m_playCount(0)
{
	m_anim = NULL;
}

CompSpriteAnim::CompSpriteAnim()
{
}

CompSpriteAnim::~CompSpriteAnim()
{
	if (m_anim)
		delete m_anim;
}

void CompSpriteAnim::Update(double dt)
{
	if (m_anim->animActive)
	{
		m_currentTime += (float)dt;

		int numFrame = Math::Max(1, m_anim->endFrame - m_anim->startFrame + 1);
		float frameTime = m_anim->animTime / numFrame;

		m_currentFrame = Math::Min(m_anim->endFrame, m_anim->startFrame + (int)(m_currentTime / frameTime));

		if (m_currentTime >= m_anim->animTime)
		{
			m_anim->ended = true;
			if (m_anim->repeatCount == 0)
			{
				m_anim->animActive = false;
				m_currentTime = 0.f;
				m_currentFrame = m_anim->startFrame;
			}
			if (m_anim->repeatCount == 1)
			{				
				m_currentTime = 0.f;
				m_currentFrame = m_anim->startFrame;
			}
		}
	}
}