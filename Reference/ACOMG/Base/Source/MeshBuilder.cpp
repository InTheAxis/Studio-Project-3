#include "MeshBuilder.h"

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
CompMesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	v.pos.Set(-1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, -1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, -1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, 1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	CompMesh *mesh = new CompMesh;
	
	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = CompMesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a crosshair;
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
CompMesh* MeshBuilder::GenerateCrossHair(const std::string &meshName, float colour_r, float colour_g, float colour_b, float length)
{
	// Declare the variable to store a vertex and the buffer for storing vertices
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	v.color.Set(colour_r, colour_g, colour_b);
	// Vertex #1
	v.pos.Set(-length, 0, 0);
	vertex_buffer_data.push_back(v);
	// Vertex #2
	v.pos.Set(length, 0, 0);	
	vertex_buffer_data.push_back(v);
	// Vertex #3
	v.pos.Set(0, -length, 0);	
	vertex_buffer_data.push_back(v);
	// Vertex #4
	v.pos.Set(0, length, 0);	
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	CompMesh *mesh = new CompMesh;
	
	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = CompMesh::DRAW_LINES;
	mesh->color.Set(colour_r, colour_g, colour_b);

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
CompMesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float length, int tile)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	v.pos.Set(-0.5f * length,-0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(tile * 1.0f, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(tile * 1.0f, tile * 1.0f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, tile * 1.0f);
	vertex_buffer_data.push_back(v);
	
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);
	
	CompMesh *mesh =new CompMesh;
	
	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = CompMesh::DRAW_TRIANGLES;
	mesh->color = color;

	return mesh;
}

CompMesh * MeshBuilder::GenerateTriangle(const std::string & meshName, Color color, float length, int tile)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.pos.Set(-0.5f * length, -0.5f * length, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, -0.5f * length, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(tile * 1.0f, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0.5f * length, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(tile * 1.0f, tile * 1.0f);
	vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);

	CompMesh *mesh = new CompMesh;

	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = CompMesh::DRAW_TRIANGLES;
	mesh->color = color;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
CompMesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	v.pos.Set(-0.5f * length,-0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length,-0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);
	v.color = color;
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
	
	CompMesh *mesh =new CompMesh;
	
	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->indexSize = 36;
	mesh->mode = CompMesh::DRAW_TRIANGLES;
	mesh->color = color;

	return mesh;
}

CompMesh* MeshBuilder::GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR, float innerR)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	Vertex v;

	float degreePerSlice = 360.f / numSlice;
	//float radianPerSlice = Math::DegreeToRadian(360.f) / numSlice;
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		float theta = slice * degreePerSlice;
		v.color = color;
		v.normal.Set(0, 1, 0);
		v.pos.Set(outerR * cos(Math::DegreeToRadian(theta)), 0, outerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		
		v.color = color;
		v.normal.Set(0, 1, 0);
		v.pos.Set(innerR * cos(Math::DegreeToRadian(theta)), 0, innerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		index_buffer_data.push_back(2 * slice + 0);
		index_buffer_data.push_back(2 * slice + 1);
		//index_buffer_data.push_back(2 * slice + 3);
		//index_buffer_data.push_back(2 * slice + 4);
		//index_buffer_data.push_back(2 * slice + 3);
		//index_buffer_data.push_back(2 * slice + 2);
	}

	CompMesh *mesh =new CompMesh;
	
	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = CompMesh::DRAW_TRIANGLE_STRIP;
	mesh->color = color;

	return mesh;
}

float sphereX(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
}
float sphereY(float phi, float theta)
{
	return sin(Math::DegreeToRadian(phi));
}
float sphereZ(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
}

CompMesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	//float radianPerSlice = Math::DegreeToRadian(360.f) / numSlice;

	for(unsigned stack = 0; stack < numStack + 1; ++stack) //stack //replace with 180 for sphere
	{
		float phi = -90.f + stack * degreePerStack;
		for(unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
		{
			float theta = slice * degreePerSlice;
			Vertex v;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			Vector3 temp = v.pos.Normalized();
			v.texCoord.Set(atan2f(temp.x, temp.z) / (2 * Math::PI) + 0.5, temp.y * 0.5 + 0.5);
			vertex_buffer_data.push_back(v);
		}
	}
	for(unsigned stack = 0; stack < numStack; ++stack)
	{
		for(unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
			//index_buffer_data.push_back((numSlice + 1) * stack + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * stack + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	CompMesh *mesh = new CompMesh;

	
	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = CompMesh::DRAW_TRIANGLE_STRIP;
	mesh->color = color;

	return mesh;
}

CompMesh* MeshBuilder::GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	Vertex v;
	float degreePerSlice = 360.f / numSlice;

	for(unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
	{
		float theta = slice * degreePerSlice;
		Vector3 normal(height * cos(Math::DegreeToRadian(theta)), radius, height * sin(Math::DegreeToRadian(theta)));
		normal.Normalize();

		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal = normal;
		vertex_buffer_data.push_back(v);
		
		v.pos.Set(0, height, 0);
		v.color = color;
		v.normal = normal;
		vertex_buffer_data.push_back(v);
	}
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		index_buffer_data.push_back(slice * 2 + 0);
		index_buffer_data.push_back(slice * 2 + 1);
	}

	CompMesh *mesh =new CompMesh;

	
	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = CompMesh::DRAW_TRIANGLE_STRIP;
	mesh->color = color;

	return mesh;
}

CompMesh * MeshBuilder::GeneratePlane(const std::string & meshName, Color color, unsigned numRow, unsigned numCol, float tileSize)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.color = color;
	v.normal.Set(0, 1, 0);

	float texDeltaRow = 1.f / numRow;
	float texDeltaCol = 1.f / numCol;
	for (int x = 0; x <= numRow; ++x)
	{
		for (int z = 0; z <= numCol; ++z)
		{	
			float xDist = x - numRow * 0.5f;
			float zDist = z - numCol * 0.5f;
			v.pos.Set(xDist, 0, zDist);
			v.texCoord.Set(numRow / tileSize * ((float)x * texDeltaRow), numCol / tileSize * (1.f - ((float)z * texDeltaCol)));
			vertex_buffer_data.emplace_back(v);
		}
	}

	for (int x = 0; x < numRow; ++x)
	{
		for (int z = 0; z < numCol; ++z)
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

	CompMesh *mesh = new CompMesh;

	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->color = color;
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = CompMesh::DRAW_TRIANGLES;

	return mesh;
}

CompMesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path, Color color)
{
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = HelperResources::LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if(!success)
		return NULL;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	HelperResources::IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	CompMesh *mesh =new CompMesh;
	
	
	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = CompMesh::DRAW_TRIANGLES;
	mesh->color = color;

	return mesh;
}

CompMesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	float width = 1.f / numCol;
	float height = 1.f / numRow;
	int offset = 0;
	for(unsigned i = 0; i < numRow; ++i)
	{
		for(unsigned j = 0; j < numCol; ++j)
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
	
	CompMesh *mesh =new CompMesh;
	
	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = CompMesh::DRAW_TRIANGLES;

	return mesh;
}

CompMesh * MeshBuilder::GenerateSkyPlane(const std::string & meshName, Color color, int slices, float PlanetRadius, float AtmosphereRadius, float hTile, float vTile)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.color = color;

	slices = Math::Clamp(slices, 1, 256);
	float planeSize = 2.0f * sqrtf((AtmosphereRadius * AtmosphereRadius) + (PlanetRadius * PlanetRadius));
	float delta = planeSize / (float)slices;
	float texDelta = 2.0f / (float)slices;

	for (int x = 0; x <= slices; ++x)
	{
		for (int z = 0; z <= slices; ++z)
		{
			float xDist = (-0.5f * planeSize) + ((float)x * delta);
			float zDist = (-0.5f * planeSize) + ((float)z * delta);
			float xHeight = (xDist * xDist) / AtmosphereRadius;
			float zHeight = (zDist * zDist) / AtmosphereRadius;
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

	CompMesh *mesh =new CompMesh;

	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = CompMesh::DRAW_TRIANGLES;

	return mesh;
}

CompMesh* MeshBuilder::GenerateTerrain(const std::string &meshName, const std::string &file_path, std::vector<unsigned char> &heightMap, float scaleX, float scaleY, float scaleZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	// Set a scale factor to size your terrain
	const float SCALE_FACTOR = 256.f;
	// Load/ read information from the heightmap 
	if (!HelperResources::LoadHeightMap(file_path.c_str(), heightMap))
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

	CompMesh *mesh =new CompMesh;

	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = CompMesh::DRAW_TRIANGLES;

	return mesh;
}

CompSpriteAnim * MeshBuilder::GenerateSpriteAnimation(const std::string & meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

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

	CompSpriteAnim *mesh = new CompSpriteAnim(numRow, numCol);

	GenerateVao(mesh, vertex_buffer_data, index_buffer_data);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = CompMesh::DRAW_TRIANGLES;

	return mesh;
}

void MeshBuilder::GenerateVao(CompMesh* mesh, std::vector<Vertex>& vertex_buffer_data, std::vector<unsigned>& index_buffer_data)
{	
	glGenVertexArrays(1, &(mesh->vao));	
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
	//normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
	glEnableVertexAttribArray(2);
	//texture coords
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Normal)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}



