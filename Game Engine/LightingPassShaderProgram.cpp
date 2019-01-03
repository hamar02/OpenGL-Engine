#include "LightingPassShaderProgram.h"

LightingPassShaderProgram::LightingPassShaderProgram()
{
	createShaderData();

}


LightingPassShaderProgram::~LightingPassShaderProgram()
{
}

void LightingPassShaderProgram::createShaderData()
{

	ShaderProgram temp = ShaderProgram();
	screenQuadShader = temp.CreateShaderData("", "", "", "", "", "DeferredLightingPassCS.glsl");


	////create vertices
	//QuadVertex vertices[] = {
	//	// pos and normal and uv for each vertex
	//	{ 2 * 0.5f, 2 * -0.5f, 1.0f, 0.0f },
	//	{ 2 * 0.5f, 2 * 0.5f, 1.0f, 1.0f },
	//	{ 2 * -0.5f, 2 * -0.5f, 0.0f, 0.0f },
	//	{ 2 * -0.5f, 2 * 0.5f, 0.0f, 1.0f },

	//};



	//unsigned int indices[] = {
	//	0,1,2,
	//	2,1,3,

	//};

	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//unsigned int vertexPos = glGetAttribLocation(screenQuadShader, "aPos");

	//if (vertexPos == -1) {
	//	OutputDebugStringA("Error, cannt find aPos attribute in vertex shader\n");
	//	return;
	//}

	//glVertexAttribPointer(
	//	vertexPos,
	//	2,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(QuadVertex),
	//	BUFFER_OFFSET(0)
	//);

	//unsigned int uvPos = glGetAttribLocation(screenQuadShader, "aTexCoords");

	//if (uvPos == -1) {
	//	OutputDebugStringA("Error, cannt find aTexCoords attribute in vertex shader\n");
	//	return;
	//}

	//glVertexAttribPointer(
	//	uvPos,
	//	2,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(QuadVertex),
	//	BUFFER_OFFSET(sizeof(float) * 2)
	//);

	////ebo
	//glGenBuffers(1, &ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void LightingPassShaderProgram::activateShaderProgram()
{
	glUseProgram(screenQuadShader);
}
