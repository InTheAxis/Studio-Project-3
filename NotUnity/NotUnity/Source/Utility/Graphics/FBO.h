#ifndef FBO_H
#define FBO_H

//has color depth and stencil attachment

#include <GL/glew.h>
#include "../WinDebug.h"

class FBO
{
public:
	FBO();
    ~FBO();

    bool Init(unsigned width, unsigned height);
    void BindForWriting();
    void BindForReading(GLenum textureUnit);

	GLuint GetFBO() {return m_fbo;}
	GLuint GetTexture() {return m_texture;}
	unsigned GetWidth() {return m_width;}
	unsigned GetHeight() {return m_height;}

	static void BindDefault();

protected:
    GLuint m_fbo;
	GLuint m_rbo;
	GLuint m_texture;
	unsigned m_width;
	unsigned m_height;
};

#endif
