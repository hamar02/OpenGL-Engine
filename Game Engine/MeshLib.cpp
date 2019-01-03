#include "MeshLib.h"

MeshLib::MeshLib()
{
}

MeshLib::MeshLib(AssetManager *otherAssetManager)
{
	assetManager = otherAssetManager;

}

MeshLib::~MeshLib()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}
}

int MeshLib::addMesh(std::string filePath, GLuint otherShaderProgram)
{
	Mesh* newMesh =new Mesh(filePath, assetManager->createNewAsset(), otherShaderProgram);
	meshes.push_back(newMesh);

	return meshes.size()-1;
}

 Mesh* MeshLib::getMesh(int key)
{
	return meshes.at(key);
}
