#pragma once
#include <GL/gl3w.h>  
#include "stb_image.h"
#include <string>
#include<iostream>

class Texture
{
public:
	Texture();
	Texture(std::string filePath, int otherAssetID);
	~Texture();

	void CreateTextureData(std::string filePath);
	void CreateHDRTextureData(std::string filePath);

	GLuint gTexture;
	int assetID;
private:
	int width, height, nrChannels = 0;

};

