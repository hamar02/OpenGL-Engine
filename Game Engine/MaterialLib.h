#pragma once
#include<vector>
#include"Material.h"
#include<string>
#include"AssetManager.h"

class MaterialLib
{
public:
	MaterialLib();
	MaterialLib(AssetManager *otherAssetManager);

	~MaterialLib();

	int addMaterial(GLuint otherShaderProgram);
	Material* getMaterial(int key);
	AssetManager *assetManager;
private:
	std::vector<Material*> materials;
};