#pragma once
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
#include <fstream>
#include <GL/gl3w.h>  
#include "QuadVertex.h"
#include<iostream>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <windows.h>
#include "ShaderProgram.h"

class SSAOShaderProgram
{
public:
	GLuint gShaderProgram;
	unsigned int ebo;
	unsigned int vbo;
	unsigned int vao;
	unsigned int noiseTexture;
	std::vector<glm::vec3> ssaoKernel;
	unsigned int ssaoFBO;
	unsigned int ssaoColorBuffer;

	SSAOShaderProgram();
	~SSAOShaderProgram();

	void createShaderData();
	void createFBO();
	void activateShaderProgram();
private:
	float lerp(float a, float b, float f);
};