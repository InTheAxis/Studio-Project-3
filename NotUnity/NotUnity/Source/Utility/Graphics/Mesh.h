#ifndef COMP_MESH_H
#define COMP_MESH_H

#include "GL/glew.h"

class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES = 0,
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};

	Mesh()
		: name("unnamed")
		, mode(DRAW_TRIANGLES)
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
	DRAW_MODE mode;
	unsigned indexSize;
	unsigned vbo;
	unsigned ebo;
	unsigned vao;


	//Material material;
};

#endif