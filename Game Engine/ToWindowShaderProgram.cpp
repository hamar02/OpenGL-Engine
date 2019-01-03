#include "ToWindowShaderProgram.h"

ToWindowShaderProgram::ToWindowShaderProgram()
{
	createShaderData();
}

ToWindowShaderProgram::~ToWindowShaderProgram()
{
}

void ToWindowShaderProgram::createShaderData()
{
	ShaderProgram temp = ShaderProgram();
	gShaderProgram = temp.CreateShaderData("ToWindowVS.glsl", "", "", "", "ToWindowFS.glsl", "");


	//create vertices
	QuadVertex vertices[] = {
		// pos and normal and uv for each vertex
		{ 2 * 0.5f, 2 * -0.5f, 1.0f, 0.0f },
		{ 2 * 0.5f, 2 * 0.5f, 1.0f, 1.0f },
		{ 2 * -0.5f, 2 * -0.5f, 0.0f, 0.0f },
		{ 2 * -0.5f, 2 * 0.5f, 0.0f, 1.0f },

	};



	unsigned int indices[] = {
		0,1,2,
		2,1,3,

	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int vertexPos = glGetAttribLocation(gShaderProgram, "vertex_position");

	if (vertexPos == -1) {
		OutputDebugStringA("Error, cannt find aPos attribute in vertex shader\n");
		return;
	}

	glVertexAttribPointer(
		vertexPos,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(QuadVertex),
		BUFFER_OFFSET(0)
	);

	unsigned int uvPos = glGetAttribLocation(gShaderProgram, "uvCoord");

	if (uvPos == -1) {
		OutputDebugStringA("Error, cannt find aTexCoords attribute in vertex shader\n");
		return;
	}

	glVertexAttribPointer(
		uvPos,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(QuadVertex),
		BUFFER_OFFSET(sizeof(float) * 2)
	);

	//ebo
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void ToWindowShaderProgram::draw(GLuint inRBO)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(gShaderProgram);
	glBindVertexArray(vao);

	glUniform1i(glGetUniformLocation(gShaderProgram, "textureColorIn"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inRBO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
