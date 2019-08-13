#ifndef FLOAT_FBO_H
#define FLOAT_FBO_H

#include <GL/glew.h>

#include "FBO.h"

class FloatFBO : public FBO
{
public:
    FloatFBO();
    ~FloatFBO();

    virtual bool Init(unsigned width, unsigned height);
};

#endif
