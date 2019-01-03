#include "MeshFilter.h"

MeshFilter::MeshFilter():Component()
{
	assetName = "MeshFilter";

}

MeshFilter::MeshFilter(GLuint otherVertexBuffer, GLuint otherVertexArray, GLuint otherElementBuffer, int otherVertexCount):Component(assetID)
{
	assetName = "MeshFilter";
	gVertexBuffer = otherVertexBuffer;
	gVertexArray =otherVertexArray;
	gElementBuffer=otherElementBuffer;
	vertexCount = otherVertexCount;
}

MeshFilter::~MeshFilter()
{
}

void MeshFilter::update()
{
}

void MeshFilter::bindVertexArray()
{
	glBindVertexArray(gVertexArray);
}

