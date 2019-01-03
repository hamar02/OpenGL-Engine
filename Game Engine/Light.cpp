#include "Light.h"
#include"GameObject.h"

Light::Light():Component()
{
	//Wrong
}

Light::Light(unsigned int otherShaderProgram, LIGHT_TYPES otherLightType)
{
	assetName = "Light";
	lightType = otherLightType;
	range = 10;
	spotAngle = 30;
	color = glm::vec4(1, 1, 1, 1);
	intensity = 1;
	depthMapFBO = 0;
	shadowType = SHADOW_TYPE::SoftShadows;
	shadowStrength = 1;
	shadowmapResolution = TEXTURE_RESOLUTIONS::R2048x2048;
	setShadowMapResolution(shadowmapResolution);
	gShaderProgram = otherShaderProgram;
}

Light::~Light()
{
}

void Light::update()
{
}

void Light::setShadowMapResolution(TEXTURE_RESOLUTIONS resolution)
{
	int widht = 0;
	int height = 0;

	switch (resolution)
	{
	case TEXTURE_RESOLUTIONS::R256x256:
		widht = 256;
		height = 256;
		break;
	case TEXTURE_RESOLUTIONS::R512x512:
		widht = 512;
		height = 512;
		break;
	case TEXTURE_RESOLUTIONS::R1024x1024:
		widht = 1024;
		height = 1024;
		break;
	case TEXTURE_RESOLUTIONS::R2048x2048:
		widht = 2048;
		height = 2048;
		break;
	default:
		break;

	}
	SHADOW_WIDTH = widht;
	SHADOW_HEIGHT = height;

	if (depthMapFBO == 0) {
		glGenFramebuffers(1, &depthMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR FBO LIGHT" << std::endl;
	}
	else
	{
		//delete shadowMap;
		glGenFramebuffers(1, &depthMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR FBO LIGHT" << std::endl;
	}
}

void Light::setupShadowMap()
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(gShaderProgram);
	calculateMatrices();

}

void Light::calculateMatrices()
{
	float nearPlane = 1.0f, farPlane = 7.5f;
	glm::mat4 lightView;
	glm::mat4 lightProjection;
	//light space matrix
	switch (lightType)
	{
	case LIGHT_TYPES::Spot:
		break;
	case LIGHT_TYPES::Directional:
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
		//lightView = glm::lookAt(gameObject->transform.position + gameObject->transform.up, glm::vec3(0,0,0),gameObject->transform.forward);
		gameObject->transform.position = glm::vec3(-2.0f, 4.0f, -1.0f);
		lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		lightSpaceMatrix = lightProjection*lightView;
		break;
	case LIGHT_TYPES::Point:
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
		lightView = glm::lookAt(gameObject->transform.position, gameObject->transform.position - gameObject->transform.up, gameObject->transform.forward);
		lightSpaceMatrix = lightProjection*lightView;
		break;
	default:
		break;
	}
	//lightSpaceMatrix = glm::mat4(10);
	//GLuint uniformLoc = glGetUniformLocation(gShaderProgram, "lightSpace_matrix");
	//glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	//std::cout << "Light Matrix"<< std::endl << lightSpaceMatrix[0][0] <<" "<< lightSpaceMatrix[1][0] << " " << lightSpaceMatrix[2][0] << " " << lightSpaceMatrix[3][0] << std::endl << lightSpaceMatrix[0][1] << " " << lightSpaceMatrix[1][1] << " " << lightSpaceMatrix[2][1] << " " << lightSpaceMatrix[3][1] <<std::endl<< lightSpaceMatrix[0][2] << " " << lightSpaceMatrix[1][2] << " " << lightSpaceMatrix[2][2] << " " << lightSpaceMatrix[3][2] << std::endl << lightSpaceMatrix[0][3] << " " << lightSpaceMatrix[1][3] << " " << lightSpaceMatrix[2][3] << " " << lightSpaceMatrix[3][3] << std::endl;

}

void Light::uniformLightSpaceMatrix(unsigned int otherShaderProgram)
{
	GLuint uniformLoc = glGetUniformLocation(otherShaderProgram, "lightSpace_matrix");
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

}
