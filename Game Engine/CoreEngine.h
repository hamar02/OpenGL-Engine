#pragma once
#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>

/////////////
#include "MaterialLib.h"
#include "TextureLib.h"
#include "MeshLib.h"
#include"AssetManager.h"
#include"GameScene.h"
#include "RenderManager.h"
#include"Transform.h"
#include "MeshFilter.h"
#include "CharacterMovement.h"
#include "Camera.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Light.h"
#include <ctime>
#include "shadowMapShaderProgram.h"
#include <algorithm>  
#include "LIGHT_TYPES.h"
#include "ToWindowShaderProgram.h"
#include "EditorGUI.h"
#include "FrustumCullingShaderProgram.h"
#include "DrawLineShaderProgram.h"
#include "GBufferShaderProgram.h"
#include <algorithm>    

#define _CRTDBG_MAP_ALLOC
class CoreEngine
{
public:
	CoreEngine();
	~CoreEngine();

	void Run();
	void importOBJFile(std::string filepath, unsigned int shaderProgram, int &meshID, int &materialID);

private:
	GLFWwindow* window;
	GLFWwindow* window2;

	AssetManager assetManager;
	GameScene gameScene;
	MaterialLib materialLibrary;
	TextureLib textureLibrary;
	MeshLib meshLibrary;
	std::vector<GameObject*> lightsToRender;

	int importMTLFile(std::string filepath, unsigned int shaderProgram);
	void FindObjectsToRender();

};
