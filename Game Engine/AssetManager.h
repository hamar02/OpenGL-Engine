#pragma once
#include <vector>
#include <windows.h>
#include <string>



class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	std::vector<int> occupiedAssetIDs;
	std::vector<int> unoccupiedAssetIDs;
	int capacity;

	int createNewAsset();
	void deleteAsset(int assetID);
private:
	int findNextAssetID();
};
