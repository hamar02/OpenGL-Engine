#include "SSAOShaderProgram.h"

SSAOShaderProgram::SSAOShaderProgram()
{
	createShaderData();
	createFBO();
}

SSAOShaderProgram::~SSAOShaderProgram()
{
}

void SSAOShaderProgram::createShaderData()
{
	ShaderProgram temp = ShaderProgram();
	gShaderProgram = temp.CreateShaderData("ssaoVS.glsl", "", "", "", "ssaoFS.glsl", "");


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

void SSAOShaderProgram::createFBO()
{
	//FBO
	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	// SSAO color buffer
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 1280, 720, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Framebuffer not complete!" << std::endl;

	//Noise and kernel
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;
	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0;

		// scale samples s.t. they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	// generate noise texture
	// ----------------------
	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

void SSAOShaderProgram::activateShaderProgram()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(vao);
	glUseProgram(gShaderProgram);
}

float SSAOShaderProgram::lerp(float a, float b, float f)
{
	return a + f * (b - a);
}
