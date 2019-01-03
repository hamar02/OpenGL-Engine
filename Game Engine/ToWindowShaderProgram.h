#pragma once
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
#include <fstream>
#include <GL/gl3w.h>  
#include "QuadVertex.h"
#include <windows.h>
#include "ShaderProgram.h"

class ToWindowShaderProgram
{
public:
	GLuint gShaderProgram;
	unsigned int ebo;
	unsigned int vbo;
	unsigned int vao;

	ToWindowShaderProgram();
	~ToWindowShaderProgram();

	void createShaderData();
	void draw(GLuint inRBO);
private:

};
