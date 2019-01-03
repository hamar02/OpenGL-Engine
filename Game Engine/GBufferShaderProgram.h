#pragma once


#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include "Vertex.h"
#include <GL/gl3w.h>  
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")


// macro that returns "char*" with offset "i"
// BUFFER_OFFSET(5) transforms in "(char*)nullptr+(5)"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))


class GBufferShaderProgram
{
public:
	GBufferShaderProgram();
	GBufferShaderProgram(int otherAssetID);
	~GBufferShaderProgram();

	void CreateShaderData();
	GLuint gShaderProgram;
	int assetID;


};

