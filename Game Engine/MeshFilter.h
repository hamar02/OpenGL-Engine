#pragma once
#include <GL/gl3w.h>  
#include"Component.h"

class MeshFilter :public Component
{
public:
	MeshFilter();
	MeshFilter(GLuint gVertexBuffer, GLuint gVertexArray, GLuint gElementBuffer, int otherVertexCount);
	~MeshFilter();

	GLuint gVertexBuffer;
	GLuint gVertexArray;
	GLuint gElementBuffer;
	int vertexCount;

	void update();
	void bindVertexArray();
private:

};

