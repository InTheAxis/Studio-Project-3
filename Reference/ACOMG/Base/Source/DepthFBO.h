#ifndef DEPTH_FBO_H
#define DEPTH_FBO_H

#include "FBO.h"

#include <GL/glew.h>

class DepthFBO : public FBO
{
public:
    DepthFBO();
    ~DepthFBO();

    virtual bool Init(unsigned width, unsigned height);
};

#endif
