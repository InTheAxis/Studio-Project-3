#ifndef MESHBUILDER_H
#define MESHBUILDER_H

#include <vector>
#include <iostream>

#include <GL\glew.h>
#include "Vertex.h"
#include "Mesh.h"
#include "../Resource.h"
#include "../Math/MyMath.h"
#include "../Manager/MgrGraphics.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
namespace MeshBuilder
{
	//generates a vao for the given mesh
	inline void BindVao(Mesh* mesh, std::vector<Vertex>& vertex_buffer_data, std::vector<unsigned>& index_buffer_data)
	{
		glBindVertexArray(mesh->vao);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
		glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

		//vertex position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		//vertex color
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position)));
		glEnableVertexAttribArray(1);
		//normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
		glEnableVertexAttribArray(2);
		//texture coords
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Normal)));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);
	}

	inline Mesh* GenerateAxes(const std::string &meshName, float lengthX = 1.0f, float lengthY = 1.0f, float lengthZ = 1.0f, bool cache = true)
	{
		if (MgrGraphics::Instance()->GetCachedMesh(meshName)) return MgrGraphics::Instance()->GetCachedMesh(meshName);
		Vertex v;
		std::vector<Vertex> vertex_buffer_data;
		v.pos.Set(-1000, 0, 0);
		v.color.Set(1, 0, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(1000, 0, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, -1000, 0);
		v.color.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0, 1000, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, 0, -1000);
		v.color.Set(0, 0, 1);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0, 0, 1000);
		vertex_buffer_data.push_back(v);

		std::vector<GLuint> index_buffer_data;
		index_buffer_data.push_back(0);
		index_buffer_data.push_back(1);
		index_buffer_data.push_back(2);
		index_buffer_data.push_back(3);
		index_buffer_data.push_back(4);
		index_buffer_data.push_back(5);

		Mesh *mesh = new Mesh;

		BindVao(mesh, vertex_buffer_data, index_buffer_data);

		mesh->name = meshName;
		mesh->indexSize = index_buffer_data.size();
		mesh->drawMode = GL_LINES;
		if (cache) MgrGraphics::Instance()->CacheMesh(mesh);

		return mesh;
	}

	inline Mesh* GenerateQuad(const std::string &meshName, float tile = 1.f, bool cache = true)
	{
		if (MgrGraphics::Instance()->GetCachedMesh(meshName)) return MgrGraphics::Instance()->GetCachedMesh(meshName);
		Vertex v;
		std::vector<Vertex> vertex_buffer_data;
		v.normal.Set(0, 0, 1);
		v.color.Set(1, 1, 1);

		v.pos.Set(-0.5f, -0.5f, 0);
		v.texCoord.Set(0, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f, -0.5f, 0);
		v.texCoord.Set(tile * 1.0f, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f, 0.5f, 0);
		v.texCoord.Set(tile * 1.0f, tile * 1.0f);
		vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f, 0.5f, 0);
		v.texCoord.Set(0, tile * 1.0f);
		vertex_buffer_data.push_back(v);

		std::vector<GLuint> index_buffer_data;
		index_buffer_data.push_back(3);
		index_buffer_data.push_back(0);
		index_buffer_data.push_back(2);
		index_buffer_data.push_back(1);
		index_buffer_data.push_back(2);
		index_buffer_data.push_back(0);

		Mesh *mesh = new Mesh;

		BindVao(mesh, vertex_buffer_data, index_buffer_data);

		mesh->name = meshName;
		mesh->indexSize = index_buffer_data.size();
		mesh->drawMode = GL_TRIANGLES;
		if (cache) MgrGraphics::Instance()->CacheMesh(mesh);

		return mesh;
	}

	inline Mesh* GenerateTriangle(const std::string &meshName, bool cache = true)
	{
		if (MgrGraphics::Instance()->GetCachedMesh(meshName)) return MgrGraphics::Instance()->GetCachedMesh(meshName);
		Vertex v;
		std::vector<Vertex> vertex_buffer_data;
		v.normal.Set(0, 0, 1);
		v.color.Set(1, 1, 1);

		v.pos.Set(-0.5f, -0.5f, 0);
		v.texCoord.Set(0, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f, -0.5f, 0);
		v.texCoord.Set(1.0f, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0, 0.5f, 0);
		v.texCoord.Set(1.0f, 1.0f);
		vertex_buffer_data.push_back(v);

		std::vector<GLuint> index_buffer_data;
		index_buffer_data.push_back(1);
		index_buffer_data.push_back(2);
		index_buffer_data.push_back(0);

		Mesh *mesh = new Mesh;

		BindVao(mesh, vertex_buffer_data, index_buffer_data);

		mesh->name = meshName;
		mesh->indexSize = index_buffer_data.size();
		mesh->drawMode = GL_TRIANGLES;
		if (cache) MgrGraphics::Instance()->CacheMesh(mesh);

		return mesh;
	}

	inline Mesh* GeneratePolygon(const std::string &meshName, unsigned numSides = 3, bool cache = true)
	{
		if (MgrGraphics::Instance()->GetCachedMesh(meshName)) return MgrGraphics::Instance()->GetCachedMesh(meshName);
		Vertex v;
		std::vector<Vertex> vertex_buffer_data;

		float deltaTheta = 360.f / numSides;
		Position pos;
		float rad;
		v.normal.Set(0, 0, 1);
		v.color.Set(1, 1, 1);

		v.pos.SetZero();
		v.texCoord.Set(0.5f, 0.5f);
		vertex_buffer_data.push_back(v);
		for (float i = 0; i < 360.f; i += deltaTheta)
		{
			rad = Math::DegreeToRadian(i);
			pos.Set(cosf(rad), sinf(rad), 0);
			v.pos = 0.5f * pos;
			v.texCoord.Set(pos.x, pos.y);
			vertex_buffer_data.push_back(v);
		}

		std::vector<GLuint> index_buffer_data;
		for (unsigned i = 0; i < numSides; ++i)
		{
			index_buffer_data.push_back(0);
			index_buffer_data.push_back(i);
			index_buffer_data.push_back(i + 1);
		}

		Mesh *mesh = new Mesh;

		BindVao(mesh, vertex_buffer_data, index_buffer_data);

		mesh->name = meshName;
		mesh->indexSize = index_buffer_data.size();
		mesh->drawMode = GL_TRIANGLES;
		if (cache) MgrGraphics::Instance()->CacheMesh(mesh);

		return mesh;
	}

	inline Mesh* GeneratePlane(const std::string &meshName, unsigned numRow = 10, unsigned numCol = 10, float tileSize = 1.f, bool cache = true)
	{
		if (MgrGraphics::Instance()->GetCachedMesh(meshName)) return MgrGraphics::Instance()->GetCachedMesh(meshName);
		Vertex v;
		std::vector<Vertex> vertex_buffer_data;
		std::vector<GLuint> index_buffer_data;


		float texDeltaRow = 1.f / numRow;
		float texDeltaCol = 1.f / numCol;

		v.normal.Set(0, 1, 0);
		v.color.Set(1, 1, 1);

		for (unsigned x = 0; x <= numRow; ++x)
		{
			for (unsigned z = 0; z <= numCol; ++z)
			{
				float xDist = x - numRow * 0.5f;
				float zDist = z - numCol * 0.5f;
				v.pos.Set(xDist, 0, zDist);
				v.texCoord.Set(numRow / tileSize * ((float)x * texDeltaRow), numCol / tileSize * (1.f - ((float)z * texDeltaCol)));
				vertex_buffer_data.emplace_back(v);
			}
		}

		for (unsigned x = 0; x < numRow; ++x)
		{
			for (unsigned z = 0; z < numCol; ++z)
			{
				int startVert = x * (numRow + 1) + z;
				index_buffer_data.emplace_back(startVert);
				index_buffer_data.emplace_back(startVert + 1);
				index_buffer_data.emplace_back(startVert + numRow + 1);

				index_buffer_data.emplace_back(startVert + numRow + 1);
				index_buffer_data.emplace_back(startVert + 1);
				index_buffer_data.emplace_back(startVert + numRow + 2);
			}
		}

		Mesh *mesh = new Mesh;

		BindVao(mesh, vertex_buffer_data, index_buffer_data);

		mesh->name = meshName;
		mesh->indexSize = index_buffer_data.size();
		mesh->drawMode = GL_TRIANGLES;
		if (cache) MgrGraphics::Instance()->CacheMesh(mesh);

		return mesh;
	}

	inline Mesh* GenerateCube(const std::string &meshName, bool cache = true)
	{
		if (MgrGraphics::Instance()->GetCachedMesh(meshName)) return MgrGraphics::Instance()->GetCachedMesh(meshName);
		Vertex v;
		std::vector<Vertex> vertex_buffer_data;

		v.normal.Set(0, 0, 1);
		v.color.Set(1, 1, 1);

		v.pos.Set(-0.5f, -0.5f, -0.5f);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f, -0.5f, -0.5f);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f, 0.5f, -0.5f);
		vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f, 0.5f, -0.5f);
		vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f, -0.5f, 0.5f);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f, -0.5f, 0.5f);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f, 0.5f, 0.5f);
		vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f, 0.5f, 0.5f);
		vertex_buffer_data.push_back(v);

		std::vector<GLuint> index_buffer_data;
		index_buffer_data.push_back(7);
		index_buffer_data.push_back(4);
		index_buffer_data.push_back(6);
		index_buffer_data.push_back(5);
		index_buffer_data.push_back(6);
		index_buffer_data.push_back(4);
		index_buffer_data.push_back(6);
		index_buffer_data.push_back(5);
		index_buffer_data.push_back(2);
		index_buffer_data.push_back(1);
		index_buffer_data.push_back(2);
		index_buffer_data.push_back(5);
		index_buffer_data.push_back(3);
		index_buffer_data.push_back(7);
		index_buffer_data.push_back(2);
		index_buffer_data.push_back(6);
		index_buffer_data.push_back(2);
		index_buffer_data.push_back(7);
		index_buffer_data.push_back(2);
		index_buffer_data.push_back(1);
		index_buffer_data.push_back(3);
		index_buffer_data.push_back(0);
		index_buffer_data.push_back(3);
		index_buffer_data.push_back(1);
		index_buffer_data.push_back(3);
		index_buffer_data.push_back(0);
		index_buffer_data.push_back(7);
		index_buffer_data.push_back(4);
		index_buffer_data.push_back(7);
		index_buffer_data.push_back(0);
		index_buffer_data.push_back(1);
		index_buffer_data.push_back(5);
		index_buffer_data.push_back(0);
		index_buffer_data.push_back(4);
		index_buffer_data.push_back(0);
		index_buffer_data.push_back(5);

		Mesh *mesh = new Mesh;

		BindVao(mesh, vertex_buffer_data, index_buffer_data);

		mesh->name = meshName;
		mesh->indexSize = 36;
		mesh->drawMode = GL_TRIANGLES;
		if (cache) MgrGraphics::Instance()->CacheMesh(mesh);

		return mesh;
	}

	inline Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 1.f, bool cache = true)
	{
		if (MgrGraphics::Instance()->GetCachedMesh(meshName)) return MgrGraphics::Instance()->GetCachedMesh(meshName);
		Vertex v;
		std::vector<Vertex> vertex_buffer_data;
		std::vector<GLuint> index_buffer_data;

		float degreePerStack = 180.f / numStack;
		float degreePerSlice = 360.f / numSlice;
		v.color.Set(1, 1, 1);
		for (unsigned stack = 0; stack < numStack + 1; ++stack)
		{
			float phi = -90.f + stack * degreePerStack;
			for (unsigned slice = 0; slice < numSlice + 1; ++slice)
			{
				float theta = slice * degreePerSlice;
				Position pos(cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)),
					sin(Math::DegreeToRadian(phi)),
					cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)));
				v.pos = 0.5f * pos;
				v.normal = pos;
				v.texCoord.Set(atan2f(pos.x, pos.z) / (2 * Math::PI) + 0.5f, pos.y * 0.5f + 0.5f);
				vertex_buffer_data.push_back(v);
			}
		}

		for (unsigned stack = 0; stack < numStack; ++stack)
		{
			for (unsigned slice = 0; slice < numSlice + 1; ++slice)
			{
				index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
				index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
			}
		}

		Mesh *mesh = new Mesh;

		BindVao(mesh, vertex_buffer_data, index_buffer_data);

		mesh->indexSize = index_buffer_data.size();
		mesh->drawMode = GL_TRIANGLE_STRIP;
		if (cache) MgrGraphics::Instance()->CacheMesh(mesh);

		return mesh;
	}

	inline Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path, bool cache = true)
	{
		if (MgrGraphics::Instance()->GetCachedMesh(meshName)) return MgrGraphics::Instance()->GetCachedMesh(meshName);
		std::vector<Position> vertices;
		std::vector<TexCoord> uvs;
		std::vector<Vector3> normals;
		bool success = Resource::LoadOBJ(file_path.c_str(), vertices, uvs, normals);
		if (!success)
			return nullptr;

		std::vector<Vertex> vertex_buffer_data;
		std::vector<GLuint> index_buffer_data;

		Resource::IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

		Mesh *mesh = new Mesh;

		BindVao(mesh, vertex_buffer_data, index_buffer_data);

		mesh->name = meshName;
		mesh->indexSize = index_buffer_data.size();
		mesh->drawMode = GL_TRIANGLES;
		if (cache) MgrGraphics::Instance()->CacheMesh(mesh);

		return mesh;
	}

	inline Mesh* GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol, bool cache = true)
	{
		if (MgrGraphics::Instance()->GetCachedMesh(meshName)) return MgrGraphics::Instance()->GetCachedMesh(meshName);
		Vertex v;
		std::vector<Vertex> vertex_buffer_data;
		std::vector<GLuint> index_buffer_data;

		v.normal.Set(0, 0, 1);
		v.color.Set(1, 1, 1);

		float width = 1.f / numCol;
		float height = 1.f / numRow;
		int offset = 0;
		for (unsigned i = 0; i < numRow; ++i)
		{
			for (unsigned j = 0; j < numCol; ++j)
			{
				float u1 = j * width;
				float v1 = 1.f - height - i * height;
				v.pos.Set(-0.5f, -0.5f, 0);
				v.texCoord.Set(u1, v1);
				vertex_buffer_data.push_back(v);

				v.pos.Set(0.5f, -0.5f, 0);
				v.texCoord.Set(u1 + width, v1);
				vertex_buffer_data.push_back(v);

				v.pos.Set(0.5f, 0.5f, 0);
				v.texCoord.Set(u1 + width, v1 + height);
				vertex_buffer_data.push_back(v);

				v.pos.Set(-0.5f, 0.5f, 0);
				v.texCoord.Set(u1, v1 + height);
				vertex_buffer_data.push_back(v);

				index_buffer_data.push_back(offset + 0);
				index_buffer_data.push_back(offset + 1);
				index_buffer_data.push_back(offset + 2);
				index_buffer_data.push_back(offset + 0);
				index_buffer_data.push_back(offset + 2);
				index_buffer_data.push_back(offset + 3);
				offset += 4;
			}
		}

		Mesh *mesh = new Mesh;

		BindVao(mesh, vertex_buffer_data, index_buffer_data);

		mesh->name = meshName;
		mesh->indexSize = index_buffer_data.size();
		mesh->drawMode = GL_TRIANGLES;
		if (cache) MgrGraphics::Instance()->CacheMesh(mesh);

		return mesh;
	}

	inline Mesh* GenerateSkyPlane(const std::string &meshName, int slices, float planetRadius, float atmosphereRadius, float hTile = 1, float vTile = 1, bool cache = true)
	{
		if (MgrGraphics::Instance()->GetCachedMesh(meshName)) return MgrGraphics::Instance()->GetCachedMesh(meshName);
		Vertex v;
		std::vector<Vertex> vertex_buffer_data;
		std::vector<GLuint> index_buffer_data;

		v.normal.Set(0, -1, 0);
		v.color.Set(1, 1, 1);

		slices = Math::Clamp(slices, 1, 256);
		float planeSize = 2.0f * sqrtf((atmosphereRadius * atmosphereRadius) + (planetRadius * planetRadius));
		float delta = planeSize / (float)slices;
		float texDelta = 2.0f / (float)slices;

		for (int x = 0; x <= slices; ++x)
		{
			for (int z = 0; z <= slices; ++z)
			{
				float xDist = (-0.5f * planeSize) + ((float)x * delta);
				float zDist = (-0.5f * planeSize) + ((float)z * delta);
				float xHeight = (xDist * xDist) / atmosphereRadius;
				float zHeight = (zDist * zDist) / atmosphereRadius;
				float height = xHeight + zHeight;

				v.pos.Set(xDist, -height, zDist);
				v.texCoord.Set(hTile * ((float)x * texDelta * 0.5f), vTile * (1.f - ((float)z * texDelta * 0.5f)));
				vertex_buffer_data.emplace_back(v);
			}
		}

		for (int x = 0; x < slices; ++x)
		{
			for (int z = 0; z < slices; ++z)
			{
				int startVert = x * (slices + 1) + z;
				index_buffer_data.emplace_back(startVert);
				index_buffer_data.emplace_back(startVert + slices + 1);
				index_buffer_data.emplace_back(startVert + 1);

				index_buffer_data.emplace_back(startVert + 1);
				index_buffer_data.emplace_back(startVert + slices + 1);
				index_buffer_data.emplace_back(startVert + slices + 2);
			}
		}

		Mesh *mesh = new Mesh;

		BindVao(mesh, vertex_buffer_data, index_buffer_data);

		mesh->name = meshName;
		mesh->indexSize = index_buffer_data.size();
		mesh->drawMode = GL_TRIANGLES;
		if (cache) MgrGraphics::Instance()->CacheMesh(mesh);

		return mesh;
	}

	inline Mesh* GenerateTerrain(const std::string &meshName, const std::string &file_path, std::vector<unsigned char> &heightMap, float scaleX = 1.f, float scaleY = 1.f, float scaleZ = 1.f, bool cache = true)
	{
		if (MgrGraphics::Instance()->GetCachedMesh(meshName)) return MgrGraphics::Instance()->GetCachedMesh(meshName);
		Vertex v;
		std::vector<Vertex> vertex_buffer_data;
		std::vector<GLuint> index_buffer_data;

		// Set a scale factor to size your terrain
		const float SCALE_FACTOR = 256.f;
		// Load/ read information from the heightmap 
		if (!Resource::LoadHeightMap(file_path.c_str(), heightMap))
			return nullptr;
		// Calculate the terrainSize 
		unsigned terrainSize = (unsigned)sqrt((double)heightMap.size());
		float scaledHeight, xVal, zVal;
		for (unsigned z = 0; z < terrainSize; ++z)
		{
			for (unsigned x = 0; x < terrainSize; ++x)
			{
				// Based on heightmap, calculate a scaledHeight
				scaledHeight = (float)heightMap[z * terrainSize + x] / SCALE_FACTOR;
				// Calculate x, y and z of each vertices   		
				xVal = (x / (float)terrainSize - 0.5f);
				zVal = (z / (float)terrainSize - 0.5f);
				v.pos.Set(xVal, scaledHeight, zVal);
				v.color.Set(scaledHeight, scaledHeight, scaledHeight);
				// Calculate the vertex coordinates 
				v.texCoord.Set(xVal * 8, 1.f - zVal * 8);
				//calculate normals
				v.normal.Set(0, 1, 0); //default
				if (x > 0 && z > 0 && x < terrainSize - 1 && z < terrainSize - 1)
				{
					Vector3 pos(xVal * scaleX, scaledHeight * scaleY, zVal * scaleZ);
					Vector3 adjv[4];
					if (x < terrainSize - 1)
						adjv[0] = Vector3(((x + 1) / (float)terrainSize - 0.5f) * scaleX, (float)heightMap[z * terrainSize + (x + 1)] / SCALE_FACTOR * scaleY, (z / (float)terrainSize - 0.5f) * scaleZ); //right
					if (x > 0)
						adjv[1] = Vector3(((x - 1) / (float)terrainSize - 0.5f) * scaleX, (float)heightMap[z * terrainSize + (x - 1)] / SCALE_FACTOR * scaleY, (z / (float)terrainSize - 0.5f) * scaleZ); //left
					if (z < terrainSize - 1)
						adjv[2] = Vector3((x / (float)terrainSize - 0.5f) * scaleX, (float)heightMap[(z + 1) * terrainSize + x] / SCALE_FACTOR * scaleY, ((z + 1) / (float)terrainSize - 0.5f) * scaleZ); //bottom
					if (z > 0)
						adjv[3] = Vector3((x / (float)terrainSize - 0.5f) * scaleX, (float)heightMap[(z - 1) * terrainSize + x] / SCALE_FACTOR * scaleY, ((z - 1) / (float)terrainSize - 0.5f) * scaleZ); //top				
					v.normal = ((adjv[3] - pos).Cross(adjv[1] - pos)) + ((adjv[0] - pos).Cross(adjv[3] - pos)) + ((adjv[1] - pos).Cross(adjv[2] - pos)) + ((adjv[2] - pos).Cross(adjv[0] - pos));
					v.normal.Normalize();
				}
				// Assign to vertex buffer 
				vertex_buffer_data.emplace_back(v);
			}
		}
		// Calculate the indices 
		for (unsigned int z = 0; z < terrainSize - 1; ++z)
		{
			for (unsigned int x = 0; x < terrainSize - 1; ++x)
			{
				int startVert = z * terrainSize + x;
				index_buffer_data.emplace_back(startVert);
				index_buffer_data.emplace_back(startVert + terrainSize);
				index_buffer_data.emplace_back(startVert + 1);

				index_buffer_data.emplace_back(startVert + 1);
				index_buffer_data.emplace_back(startVert + terrainSize);
				index_buffer_data.emplace_back(startVert + terrainSize + 1);
			}
		}

		Mesh *mesh = new Mesh;

		BindVao(mesh, vertex_buffer_data, index_buffer_data);

		mesh->name = meshName;
		mesh->indexSize = index_buffer_data.size();
		mesh->drawMode = GL_TRIANGLES;
		if (cache) MgrGraphics::Instance()->CacheMesh(mesh);

		return mesh;
	}




	//CompSpriteAnim * MeshBuilder::GenerateSpriteAnimation(const std::string & meshName, unsigned numRow, unsigned numCol)
	//{
	//	Vertex v;
	//	std::vector<Vertex> vertex_buffer_data;
	//	std::vector<GLuint> index_buffer_data;

	//	float width = 1.f / numCol;
	//	float height = 1.f / numRow;
	//	int offset = 0;
	//	for (unsigned i = 0; i < numRow; ++i)
	//	{
	//		for (unsigned j = 0; j < numCol; ++j)
	//		{
	//			float u1 = j * width;
	//			float v1 = 1.f - height - i * height;
	//			v.pos.Set(-0.5f, -0.5f, 0);
	//			v.texCoord.Set(u1, v1);
	//			vertex_buffer_data.push_back(v);

	//			v.pos.Set(0.5f, -0.5f, 0);
	//			v.texCoord.Set(u1 + width, v1);
	//			vertex_buffer_data.push_back(v);

	//			v.pos.Set(0.5f, 0.5f, 0);
	//			v.texCoord.Set(u1 + width, v1 + height);
	//			vertex_buffer_data.push_back(v);

	//			v.pos.Set(-0.5f, 0.5f, 0);
	//			v.texCoord.Set(u1, v1 + height);
	//			vertex_buffer_data.push_back(v);

	//			index_buffer_data.push_back(offset + 0);
	//			index_buffer_data.push_back(offset + 1);
	//			index_buffer_data.push_back(offset + 2);
	//			index_buffer_data.push_back(offset + 0);
	//			index_buffer_data.push_back(offset + 2);
	//			index_buffer_data.push_back(offset + 3);
	//			offset += 4;
	//		}
	//	}

	//	CompSpriteAnim *mesh = new CompSpriteAnim(numRow, numCol);

	//	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	//mesh->name = meshName;
	//	mesh->indexSize = index_buffer_data.size();
	//	mesh->mode = Mesh::DRAW_TRIANGLES;

	//	return mesh;
	//}

};

#endif