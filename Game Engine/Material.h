#pragma once
#include <GL/gl3w.h>  
#include "stb_image.h"
#include <string>
#include<iostream>
#include"Component.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material:public Component
{
public:
	Material();
	Material(GLuint otherShaderProgram, int otherAssetID);
	~Material();

	GLuint gAlbedoTexture;
	GLuint gNormalTexture;
	GLuint gAORoughMetTexture;
	GLuint gEmissiveTexture;

	GLuint gShaderProgram;

	void update();
	void bindTextures();
	void activateShaderProgram();
	void setAlbedoTexture(unsigned int otherTexture);
	void setNormalTexture(unsigned int otherTexture);
	void setAORoughMetTexture(unsigned int otherTexture);
	void setEmissiveTexture(unsigned int otherTexture);

	void setAlbedoRGB(glm::vec3 otherColor);
	void setAORoughMetRGB(glm::vec3 otherColor);
	void setEmissiveRGB(glm::vec3 otherColor);

private:
	unsigned int gAlbedoRGB;
	unsigned int gNormalRGB;
	unsigned int gAORoughMetRGB;
	unsigned int gEmissiveRGB;

	void setNormalRGB();

};

