#ifndef VERTEX_H
#define VERTEX_H

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"

//defines what a vertex stores

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
};

#endif