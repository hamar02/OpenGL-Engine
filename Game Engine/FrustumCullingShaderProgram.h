#pragma once
#include <fstream>
#include <GL/gl3w.h>  
#include <windows.h>
#include "PRIMITIVE_GEOMETRY.h"
#include "GameObject.h"
#include "Camera.h"
#include"ShaderProgram.h"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class FrustumCullingShaderProgram
{
public:
	FrustumCullingShaderProgram();
	~FrustumCullingShaderProgram();
	unsigned int gShaderProgram;
	GLuint oobbStorageBuffer;
	GLuint positionStorageBuffer;
	GLuint indiciesStorageBuffer;
	GLuint frustumPlaneStorageBuffer;

	int numberOfObjects = 10;

	void createShaderData();
	void createSSBO();
	std::vector<GameObject*> activateShaderProgram(std::vector<GameObject*> otherGameObjects, GameObject* camera, unsigned int display_w, unsigned int display_h);

private:

};
