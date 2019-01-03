#pragma once
#include<vector>
#include"Texture.h"
#include<string>
#include"AssetManager.h"

class TextureLib
{
public:
	TextureLib();
	TextureLib(AssetManager *otherAssetManager);
	~TextureLib();

	int addTexture(std::string filePath);
	const Texture* getTexture(int key);

	int addHDRTexture(std::string filePath);
	const Texture* getHDRTexture(int key);

	AssetManager *assetManager;

private:
	std::vector<Texture*> textures;
	std::vector<Texture*>HDRTextures;

};