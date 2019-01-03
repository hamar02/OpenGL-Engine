#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl3w.h>   
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <streambuf>
#include <iostream>
#include "LightingPassShaderProgram.h"
#include "GaussianBlurShaderProgram.h"
#include "MergeBloomShaderProgram.h"
#include "GameScene.h"
#include "LIGHT_TYPES.h"
#include "SSAOShaderProgram.h"
#include "Camera.h"

class RenderManager
{
public:
    RenderManager();
    RenderManager(GameScene *otherGameScene, GLFWwindow* otherWindow);
    ~RenderManager();

    GameScene *gameScene;
    void defferedRender(GLuint outFBO, GLFWwindow* otherWindow, GameObject* camera, std::vector<GameObject*> &gameObjectsToDeferredRender, std::vector<GameObject*> &lightsToRender);
	void forwardRender(GLuint outFBO, GLFWwindow* otherWindow, GameObject* camera, std::vector<GameObject*> &gameObjectsToForwardRender, std::vector<GameObject*> &lightsToRender);
	void applyPostEffects(GLuint outFBO, GLFWwindow* otherWindow, GameObject* camera);

private:
    
	LightingPassShaderProgram lightingShaderProgram;
	PostEffectsShaderProgram postEffectShaderProgram;
	MergeBloomShaderProgram MergeBloomShaderProgram;
	SSAOShaderProgram ssaoShaderProgram;

	//G-Buffer
	unsigned int gbo;
	unsigned int gAlbedo;
	unsigned int gNormal;
	unsigned int gPosition;
	unsigned int gAORoughMet;
	unsigned int gEmissive;
	unsigned int gLightSpacePosition;
	unsigned int gAttachments[5] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
	unsigned int gRbo;
	//Lighting Pass
	unsigned int LPfbo;
	unsigned int lColor;
	unsigned int lHDRColor;
	unsigned int lProcessedColor;
	unsigned int pAttachments[3] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2};

	int display_w, display_h;

	void createGBuffer();
	void createLPBuffer();
};
