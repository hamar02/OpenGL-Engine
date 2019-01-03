#include "shadowMapShaderProgram.h"

shadowMapShaderProgram::shadowMapShaderProgram()
{

	createShaderData();
}

shadowMapShaderProgram::~shadowMapShaderProgram()
{
}

void shadowMapShaderProgram::createShaderData()
{
	ShaderProgram temp = ShaderProgram();
	gShaderProgram = temp.CreateShaderData("shadowMappingVS.glsl", "", "", "", "shadowMappingFS.glsl", "");
}
