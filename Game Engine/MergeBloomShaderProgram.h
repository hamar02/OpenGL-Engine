#pragma once
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
#include <fstream>
#include <GL/gl3w.h>  
#include "QuadVertex.h"
#include "ShaderProgram.h"

class MergeBloomShaderProgram
{
public:
	GLuint gShaderProgram;
	unsigned int ebo;
	unsigned int vbo;
	unsigned int vao;

	MergeBloomShaderProgram();
	~MergeBloomShaderProgram();

	void createShaderData();

private:

};
