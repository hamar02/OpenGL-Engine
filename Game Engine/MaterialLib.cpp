#include "MaterialLib.h"

MaterialLib::MaterialLib()
{
}

MaterialLib::MaterialLib(AssetManager *otherAssetManager)
{
	assetManager = otherAssetManager;

}

MaterialLib::~MaterialLib()
{
	for (int i = 0; i < materials.size(); i++)
	{
		delete materials[i];
	}
}

int MaterialLib::addMaterial(GLuint otherShaderProgram)
{
	Material* newMat =new Material(otherShaderProgram, assetManager->createNewAsset());
	materials.push_back(newMat);

	return materials.size()-1;
}

Material * MaterialLib::getMaterial(int key)
{

	return materials.at(key);
}
