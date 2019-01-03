#pragma once
#include <fstream>
#include <GL/gl3w.h>  
#include "QuadVertex.h"
#include "ShaderProgram.h"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class LightingPassShaderProgram
{
public:
	LightingPassShaderProgram();
	~LightingPassShaderProgram();
	unsigned int screenQuadShader;
	unsigned int ebo;
	unsigned int vbo;
	unsigned int vao;
	void createShaderData();
	void activateShaderProgram();
private:
	unsigned int fbo;
};
