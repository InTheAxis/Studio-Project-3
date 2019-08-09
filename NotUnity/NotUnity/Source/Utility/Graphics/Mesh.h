#ifndef COMP_MESH_H
#define COMP_MESH_H

#include "GL/glew.h"

class Mesh
{
public:
	Mesh()
		: name("unnamed")
		, drawMode(GL_LINE)
		, indexSize(0)	
	{
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);	
		glGenVertexArrays(1, &vao);
	}
	~Mesh()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}

	std::string name;
	unsigned drawMode;
	unsigned indexSize;
	unsigned vbo;
	unsigned ebo;
	unsigned vao;
};

#endif