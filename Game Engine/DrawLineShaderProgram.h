#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include "QuadVertex.h"
#include <GL/gl3w.h>  
#include "MeshFilter.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "GameObject.h"
#include "ShaderProgram.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class DrawLineShaderProgram
{
public:
	DrawLineShaderProgram();
	~DrawLineShaderProgram();

	unsigned int gShaderProgram;
	unsigned int ebo;
	unsigned int vbo;
	unsigned int vao;

	void createShaderData();
	void drawObject(MeshFilter* objectToRender, GameObject* mainCamera, GameObject* POVCamera, unsigned int  display_w, unsigned int  display_h);
private:

};
