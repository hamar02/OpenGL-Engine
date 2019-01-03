#include "TextureLib.h"

TextureLib::TextureLib()
{
}

TextureLib::TextureLib(AssetManager *otherAssetManager)
{
	assetManager = otherAssetManager;
}

TextureLib::~TextureLib()
{
	for (int i = 0; i < textures.size(); i++)
	{
		delete textures[i];
	}
}

int TextureLib::addTexture(std::string filePath)
{
	Texture* newMat = new Texture(filePath, assetManager->createNewAsset());
	textures.push_back(newMat);

	return textures.size()-1;
}

const Texture * TextureLib::getTexture(int key)
{
	
	return textures.at(key);
}

int TextureLib::addHDRTexture(std::string filePath)
{
	Texture* newMat = new Texture(filePath, assetManager->createNewAsset());
	HDRTextures.push_back(newMat);

	return HDRTextures.size();
}

const Texture * TextureLib::getHDRTexture(int key)
{
	return HDRTextures.at(key);
}
