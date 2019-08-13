#include "SpriteAnimation.h"

#include "GL\glew.h"
#include "Vertex.h"

SpriteAnimation::SpriteAnimation(int row, int col)
	: m_row(row)
	, m_col(col)
	, m_currentTime(0)
	, m_currentFrame(0)
	, m_playCount(0)
{
	m_anim = NULL;
}

SpriteAnimation::SpriteAnimation()
{
}

SpriteAnimation::~SpriteAnimation()
{
	if (m_anim)
		delete m_anim;
}

void SpriteAnimation::Update(double dt)
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

//void SpriteAnimation::Draw()
//{
//	glBindVertexArray(vao);
//	if (textureArray[0] > 0)
//	{
//		glBindBuffer(GL_ARRAY_BUFFER, vbo);
//		glEnableVertexAttribArray(3);
//		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
//	}
//
//	if (mode == DRAW_LINES)
//		glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));
//	else if (mode == DRAW_TRIANGLE_STRIP)
//		glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));
//	else
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));
//
//	if (textureArray[0] > 0)
//	{
//		glDisableVertexAttribArray(3);
//	}
//	glBindVertexArray(0);
//}