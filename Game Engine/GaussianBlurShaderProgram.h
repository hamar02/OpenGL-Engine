#pragma once
#include <fstream>
#include <GL/gl3w.h>  
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include "ShaderProgram.h"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class PostEffectsShaderProgram
{
public:
	PostEffectsShaderProgram();
	~PostEffectsShaderProgram();
	unsigned int gShaderProgram;


	void createShaderData();
	void activateShaderProgram();

private:

};
