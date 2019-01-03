#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include "ShaderProgram.h"

#include <GL/gl3w.h>  
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class shadowMapShaderProgram 
{
public:
	shadowMapShaderProgram();
	~shadowMapShaderProgram();

	unsigned int gShaderProgram;


	void createShaderData();
private:

};
