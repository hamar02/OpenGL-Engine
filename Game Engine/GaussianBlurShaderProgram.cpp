#include "GaussianBlurShaderProgram.h"

PostEffectsShaderProgram::PostEffectsShaderProgram()
{
	createShaderData();
}

PostEffectsShaderProgram::~PostEffectsShaderProgram()
{
}

void PostEffectsShaderProgram::createShaderData()
{

	ShaderProgram temp = ShaderProgram();
	gShaderProgram = temp.CreateShaderData("", "", "", "","", "gaussianBlurCS.glsl");

}

void PostEffectsShaderProgram::activateShaderProgram()
{
	glUseProgram(gShaderProgram);
}
