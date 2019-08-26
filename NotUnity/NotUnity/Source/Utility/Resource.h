#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <GL\glew.h>
#include "Graphics/Vertex.h"
#include "Math/Vector3.h"
#include "WinDebug.h"

//this namespace handles the loading in of resources like tga, etc

namespace Resource
{
	//SHADER
	inline GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) 
	{
		GLint result = GL_FALSE;
		int infoLogLen;
		std::string vertFilePath = vertex_file_path, fragfilePath = fragment_file_path;

		// Create the shaders
		GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
		
		// Read the Vertex Shader code from the file
		std::string vertCode;
		std::ifstream vertFileStream(("Resources/" + vertFilePath).c_str(), std::ios::in);
		if (vertFileStream.is_open()) 
		{
			std::string line = "";
			while (getline(vertFileStream, line))
				vertCode += "\n" + line;
			vertFileStream.close();
		}
		else 
		{
			Debug::LogWarning("Failed to open file: " + vertFilePath);
		}
		// Read the Fragment Shader code from the file
		std::string fragCode;
		std::ifstream fragFileStream(("Resources/" + fragfilePath).c_str(), std::ios::in);
		if (fragFileStream.is_open()) {
			std::string line = "";
			while (getline(fragFileStream, line))
				fragCode += "\n" + line;
			fragFileStream.close();
		}
		else
		{
			Debug::LogWarning("Failed to open file: " + fragfilePath);
			return 0;
		}

		// Compile Vertex Shader
		char const * vertSrc = vertCode.c_str();
		glShaderSource(vertID, 1, &vertSrc, NULL);
		glCompileShader(vertID);
		// Check Vertex Shader
		glGetShaderiv(vertID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(vertID, GL_INFO_LOG_LENGTH, &infoLogLen);
		if (infoLogLen > 0)
		{
			std::vector<char> vertErrorMsg(infoLogLen + 1);
			glGetShaderInfoLog(vertID, infoLogLen, NULL, &vertErrorMsg[0]);
			Debug::LogWarning("Failed to compile " + vertFilePath + ":");
			printf("%s\n", &vertErrorMsg[0]);
		}

		// Compile Fragment Shader
		char const * fragSrc = fragCode.c_str();
		glShaderSource(fragID, 1, &fragSrc, NULL);
		glCompileShader(fragID);
		// Check Fragment Shader
		glGetShaderiv(fragID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(fragID, GL_INFO_LOG_LENGTH, &infoLogLen);
		if (infoLogLen > 0) 
		{
			std::vector<char> fragErrorMsg(infoLogLen + 1);
			glGetShaderInfoLog(fragID, infoLogLen, NULL, &fragErrorMsg[0]);
			Debug::LogWarning("Failed to compile " + fragfilePath + ":");
			printf("%s\n", &fragErrorMsg[0]);
		}

		// Link the program
		GLuint programID = glCreateProgram();
		glAttachShader(programID, vertID);
		glAttachShader(programID, fragID);
		glLinkProgram(programID);
		// Check the program
		glGetProgramiv(programID, GL_LINK_STATUS, &result);
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLen);
		if (infoLogLen > 0) 
		{
			std::vector<char> linkMsg(infoLogLen + 1);
			glGetProgramInfoLog(programID, infoLogLen, NULL, &linkMsg[0]);
			Debug::LogWarning("Failed to link: ");
			printf("%s\n", &linkMsg[0]);
		}

		glDeleteShader(vertID);
		glDeleteShader(fragID);

		return programID;
	}

	//OBJ
	namespace
	{
		struct PackedVertex
		{
			Position position;
			TexCoord uv;
			Vector3 normal;
			bool operator<(const PackedVertex that) const
			{
				return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
			};
		};
		inline bool getSimilarVertexIndex_fast(PackedVertex & packed, std::map<PackedVertex, unsigned short> & VertexToOutIndex, unsigned short & result)
		{
			std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
			if (it == VertexToOutIndex.end())
			{
				return false;
			}
			else
			{
				result = it->second;
				return true;
			}
		}
		inline void IndexVBO(std::vector<Position> & in_vertices, std::vector<TexCoord> & in_uvs, std::vector<Vector3> & in_normals, std::vector<unsigned> & out_indices, std::vector<Vertex> & out_vertices)
		{
			std::map<PackedVertex, unsigned short> VertexToOutIndex;

			// For each input vertex
			for (unsigned int i = 0; i < in_vertices.size(); ++i)
			{

				PackedVertex packed = { in_vertices[i], in_uvs[i], in_normals[i] };

				// Try to find a similar vertex in out_XXXX
				unsigned short index;
				bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

				if (found)
				{
					// A similar vertex is already in the VBO, use it instead !
					out_indices.push_back(index);
				}
				else
				{
					// If not, it needs to be added in the output data.
					Vertex v;
					v.pos.Set(in_vertices[i].x, in_vertices[i].y, in_vertices[i].z);
					v.texCoord.Set(in_uvs[i].x, in_uvs[i].y);
					v.normal.Set(in_normals[i].x, in_normals[i].y, in_normals[i].z);
					v.color.Set(1, 1, 1, 1);
					out_vertices.push_back(v);
					unsigned newindex = (unsigned)out_vertices.size() - 1;
					out_indices.push_back(newindex);
					VertexToOutIndex[packed] = newindex;
				}
			}
		}
	}
	inline bool LoadOBJ(const char *file_rel_path, std::vector<Position> & out_vertices, std::vector<TexCoord> & out_uvs, std::vector<Vector3> & out_normals)
	{
		std::string filePath = file_rel_path;
		std::ifstream fileStream(("Resources/" + filePath).c_str(), std::ios::binary);
		if (!fileStream.is_open())
		{
			Debug::LogWarning("Failed to open file: " + filePath);
			return false;
		}

		std::vector<unsigned> vertexIndices, uvIndices, normalIndices;
		std::vector<Position> temp_vertices;
		std::vector<TexCoord> temp_uvs;
		std::vector<Vector3> temp_normals;

		while (!fileStream.eof())
		{
			char buf[256];
			fileStream.getline(buf, 256);
			if (strncmp("v ", buf, 2) == 0)
			{
				Position vertex;
				sscanf_s((buf + 2), "%f%f%f", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (strncmp("vt ", buf, 3) == 0)
			{
				TexCoord tc;
				sscanf_s((buf + 2), "%f%f", &tc.x, &tc.y);
				temp_uvs.push_back(tc);
			}
			else if (strncmp("vn ", buf, 3) == 0)
			{
				Vector3 normal;
				sscanf_s((buf + 2), "%f%f%f", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (strncmp("f ", buf, 2) == 0)
			{
				unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
				int matches = sscanf_s((buf + 2), "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2],
					&vertexIndex[3], &uvIndex[3], &normalIndex[3]);

				if (matches == 9) //triangle
				{
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				}
				else if (matches == 12) //quad
				{
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);

					vertexIndices.push_back(vertexIndex[2]);
					vertexIndices.push_back(vertexIndex[3]);
					vertexIndices.push_back(vertexIndex[0]);
					uvIndices.push_back(uvIndex[2]);
					uvIndices.push_back(uvIndex[3]);
					uvIndices.push_back(uvIndex[0]);
					normalIndices.push_back(normalIndex[2]);
					normalIndices.push_back(normalIndex[3]);
					normalIndices.push_back(normalIndex[0]);
				}
				else
				{					
					Debug::LogWarning("File can't be read by parser");
					return false;
				}
			}
		}
		fileStream.close();

		// For each vertex of each triangle
		for (unsigned i = 0; i < vertexIndices.size(); ++i)
		{
			// Get the indices of its attributes
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];

			// Get the attributes thanks to the index
			Position vertex = temp_vertices[vertexIndex - 1];
			TexCoord uv = temp_uvs[uvIndex - 1];
			Vector3 normal = temp_normals[normalIndex - 1];

			// Put the attributes in buffers
			out_vertices.push_back(vertex);
			out_uvs.push_back(uv);
			out_normals.push_back(normal);
		}

		return true;
	}

	//TGA
	inline GLuint LoadTGA(const char *file_rel_path, bool anti_alias = false)				// load TGA file to memory
	{
		std::string filePath = file_rel_path;
		std::ifstream fileStream(("Resources/" + filePath).c_str(), std::ios::binary);
		if (!fileStream.is_open())
		{
			Debug::LogWarning("Failed to open file: " + filePath);
			return 0;
		}

		GLubyte		header[18];									// first 6 useful header bytes
		GLuint		bytesPerPixel;								    // number of bytes per pixel in TGA gile
		GLuint		imageSize;									    // for setting memory
		GLubyte *	data;
		GLuint		texture = 0;
		unsigned	width, height;

		fileStream.read((char*)header, 18);
		width = header[12] + header[13] * 256;
		height = header[14] + header[15] * 256;

		if (width <= 0 ||								// is width <= 0
			height <= 0 ||								// is height <=0
			(header[16] != 24 && header[16] != 32))		// is TGA 24 or 32 Bit
		{
			fileStream.close();							// close file on failure
			std::cout << "File header error.\n";
			return 0;
		}

		bytesPerPixel = header[16] / 8;						//divide by 8 to get bytes per pixel
		imageSize = width * height * bytesPerPixel;	// calculate memory required for TGA data

		data = new GLubyte[imageSize];
		fileStream.seekg(18, std::ios::beg);
		fileStream.read((char *)data, imageSize);
		fileStream.close();

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		if (bytesPerPixel == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
		else //bytesPerPixel == 4
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

		if (anti_alias)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		float maxAnisotropy = 1.f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)maxAnisotropy);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenerateMipmap(GL_TEXTURE_2D);

		delete[]data;

		return texture;
	}

	//HEIGHT MAP
	inline bool LoadHeightMap(const char *file_rel_path, std::vector<unsigned char> &heightMap)
	{
		std::string filePath = file_rel_path;
		std::ifstream fileStream(("Resources/" + filePath).c_str(), std::ios::binary);
		if (!fileStream.is_open())
		{
			Debug::LogWarning("Failed to open file: " + filePath);
			return false;
		}

		fileStream.seekg(0, std::ios::end);
		std::streampos fsize = (unsigned)fileStream.tellg();

		fileStream.seekg(0, std::ios::beg);
		heightMap.resize((unsigned)fsize);
		fileStream.read((char *)&heightMap[0], fsize);

		fileStream.close();
		return true;
	}
	inline float ReadHeightMap(std::vector<unsigned char> &heightMap, float x, float z)
	{
		if (x < -0.5f || x > 0.5f || z < -0.5f || z > 0.5f)
			return 0;
		if (heightMap.size() == 0)
			return 0;

		unsigned terrainSize = (unsigned)sqrt((double)heightMap.size());

		unsigned zCoord = (unsigned)((z + 0.5f) * terrainSize);
		unsigned xCoord = (unsigned)((x + 0.5f) * terrainSize);

		return (float)heightMap[zCoord * terrainSize + xCoord] / 256.f;
	}

};

#endif