#ifndef DEPTH_FBO_H
#define DEPTH_FBO_H

#include <GL/glew.h>

#include "../FBO.h"

//only has depth attachment

class DepthFBO : public FBO
{
public:
    DepthFBO();
    ~DepthFBO();

    virtual bool Init(unsigned width, unsigned height);
};

#endif
