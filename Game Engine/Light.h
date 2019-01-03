#pragma once
#include <GL/gl3w.h>  
#include"Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"LIGHT_TYPES.h"
#include "SHADOW_TYPE.h"
#include "TEXTURE_RESOLUTIONS.h"


class Light :public Component
{
public:
	Light();
	Light(unsigned int otherShaderProgram, LIGHT_TYPES otherLightType);
	~Light();

	LIGHT_TYPES lightType;
	float range;
	float spotAngle;
	glm::vec4 color;
	float intensity;
	SHADOW_TYPE shadowType;
	//fix settings for baked shadows in future
	float shadowStrength;
	TEXTURE_RESOLUTIONS shadowmapResolution;
	unsigned int depthMapFBO;
	unsigned int SHADOW_WIDTH, SHADOW_HEIGHT;
	unsigned int depthMap;
	unsigned int gShaderProgram;
	glm::mat4 lightSpaceMatrix;

	void update();
	void setShadowMapResolution(TEXTURE_RESOLUTIONS resolution);
	void setupShadowMap();
	void calculateMatrices();
	void uniformLightSpaceMatrix(unsigned int otherShaderProgram);
private:

};

