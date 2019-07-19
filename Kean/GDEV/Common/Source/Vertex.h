#ifndef VERTEX_H
#define VERTEX_H

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

typedef Vector3 Position;
typedef Vector4 Color;
typedef Vector3 Normal;
typedef Vector2 TexCoord;

struct Vertex 
{
	Position pos;
	Color color;
	Normal normal;
	TexCoord texCoord;
	Vertex(){}
};

#endif