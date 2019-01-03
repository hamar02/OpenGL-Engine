#include "Texture.h"

Texture::Texture()
{
}

Texture::Texture(std::string filePath, int otherAssetID)
{
	this->assetID = otherAssetID;
	CreateTextureData(filePath);
}

Texture::~Texture()
{
}

void Texture::CreateTextureData(std::string filePath)
{

	//Texture
	glGenTextures(1, &gTexture);
	glBindTexture(GL_TEXTURE_2D, gTexture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// load and generate the texture
	stbi_set_flip_vertically_on_load(true);
	const char* fileName = filePath.c_str();
	unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	if (data)
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	GLuint error = glGetError();
	error = glGetError();

}

void Texture::CreateHDRTextureData(std::string filePath)
{
	//Texture
	glGenTextures(1, &gTexture);
	glBindTexture(GL_TEXTURE_2D, gTexture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// load and generate the texture
	stbi_set_flip_vertically_on_load(true);
	const char* fileName = filePath.c_str();
	unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	if (data)
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
