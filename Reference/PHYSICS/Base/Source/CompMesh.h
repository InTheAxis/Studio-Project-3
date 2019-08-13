#ifndef COMP_MESH_H
#define COMP_MESH_H

#include "ComponentBase.h"
#include "Vector4.h"
#include "UniMaterial.h"
#include "GL/glew.h"

class CompMesh : public Component<CompMesh>
{
public:
	CompMesh()
		: mode(DRAW_TRIANGLES)
		, color(1, 1, 1, 1)
	{
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		for (int i = 0; i < MAX_TEXTURES; ++i)
			textureArray[i] = 0;		
	}
	~CompMesh()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		for (int i = 0; i < MAX_TEXTURES; ++i)
			glDeleteTextures(i, &textureArray[i]);
	}
	
	enum DRAW_MODE
	{
		DRAW_TRIANGLES = 0, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};

	DRAW_MODE mode;
	unsigned indexSize;
	unsigned vbo;
	unsigned ebo;
	unsigned vao;

	Vector4 color;
	Material material;

	static const int MAX_TEXTURES = 8;
	unsigned textureArray[MAX_TEXTURES];
};

#endif