#pragma once
// macro that returns "char*" with offset "i"
// BUFFER_OFFSET(5) transforms in "(char*)nullptr+(5)"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))



#include <windows.h>
#include <string>
#include <fstream>

#include <GL/gl3w.h>  
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

using namespace std;

// macro that returns "char*" with offset "i"
// BUFFER_OFFSET(5) transforms in "(char*)nullptr+(5)"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))


class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	GLuint CreateShaderData(std::string vertexShader, std::string tesselationControlShader, std::string tesselationEvaluationShader, std::string geometryShader, std::string fragmentShader, std::string computeShader);

};

