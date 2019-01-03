#pragma once
#include<vector>
#include"Mesh.h"
#include<string>
#include"AssetManager.h"

class MeshLib
{
public:
	MeshLib();
	MeshLib(AssetManager *otherAssetManager);

	~MeshLib();

	int addMesh(std::string filePath, GLuint otherShaderProgram);
	 Mesh* getMesh(int key);
	 AssetManager *assetManager;

private:
	std::vector<Mesh*> meshes;
};