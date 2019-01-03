#pragma once
// macro that returns "char*" with offset "i"
// BUFFER_OFFSET(5) transforms in "(char*)nullptr+(5)"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))


#include <windows.h>
#include <string>
#include <GL/gl3w.h>  
#include "Vertex.h"
#include <vector>
#include <iostream>
#include "glm\glm.hpp"
#include <algorithm>    // std::unique, std::distance

class Mesh
{
public:
	Mesh();
	Mesh(std::string filePath, int otherAssetID, GLuint otherShaderProgram);
	~Mesh();

	void CreateMeshData(std::string filePath, GLuint otherShaderProgram);


	int assetID;
	int vertexCount;
	GLuint gVertexBuffer;
	GLuint gVertexArray;
	GLuint gElementBuffer;
};

