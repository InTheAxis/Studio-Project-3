#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include <vector>
#include <iostream>
#include <GL\glew.h>

#include "Vertex.h"
#include "CompMesh.h"
#include "HelperResources.h"
#include "MyMath.h"
#include "SpriteAnimation.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static CompMesh* GenerateAxes(const std::string &meshName, float lengthX=0.0f, float lengthY=0.0f, float lengthZ=0.0f);
	static CompMesh* GenerateCrossHair(const std::string &meshName, float colour_r=1.0f, float colour_g=1.0f, float colour_b=0.0f, float length=1.0f);
	static CompMesh* GenerateQuad(const std::string &meshName, Color color, float length = 1.f, int tile = 1);
	static CompMesh* GenerateTriangle(const std::string &meshName, Color color, float length = 1.f, int tile = 1);
	static CompMesh* GenerateCube(const std::string &meshName, Color color, float length = 1.f);
	static CompMesh* GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR = 1.f, float innerR = 0.f);
	static CompMesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 1.f);
	static CompMesh* GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height);
	static CompMesh* GenerateOBJ(const std::string &meshName, const std::string &file_path, Color color);
	static CompMesh* GenerateText(const std::string &meshName, unsigned row, unsigned col);

	//TSL
	static CompMesh* GenerateSkyPlane(const std::string &meshName, Color color, int slices,float PlanetRadius, float AtmosphereRadius, float hTile = 1, float vTile = 1);
	static CompMesh* GenerateTerrain(const std::string &meshName, const std::string &file_path, std::vector<unsigned char> &heightMap);
	static SpriteAnimation* GenerateSpriteAnimation(const std::string &meshName, unsigned numRow, unsigned numCol);


	static void GenerateVao(CompMesh* mesh, std::vector<Vertex>& vertex_buffer_data, std::vector<unsigned>& index_buffer_data); //generates a vao for the given mesh
};

#endif