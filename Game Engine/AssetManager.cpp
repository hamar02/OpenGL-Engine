#include "AssetManager.h"

AssetManager::AssetManager()
{
	occupiedAssetIDs = std::vector<int>();
	unoccupiedAssetIDs = std::vector<int>();
	capacity = 0;
}

AssetManager::~AssetManager()
{
}

int AssetManager::createNewAsset()
{
	int newID = findNextAssetID();

	occupiedAssetIDs.push_back(newID);

	return newID;
}

void AssetManager::deleteAsset(int assetID)
{
	int index = -1;
	for (int i = 0; i < occupiedAssetIDs.size(); i++)
	{
		if (assetID == occupiedAssetIDs[i]) {
			index = i;
		}
	}

	if (index != -1) {
		unoccupiedAssetIDs.push_back(occupiedAssetIDs.at(index));
		occupiedAssetIDs.erase(occupiedAssetIDs.begin() + index);
	}
}

int AssetManager::findNextAssetID()
{
	int nextID = -1;
	if (unoccupiedAssetIDs.size() > 0) {
		nextID = unoccupiedAssetIDs.front();
		unoccupiedAssetIDs.erase(unoccupiedAssetIDs.begin());
	}
	else {
		nextID = capacity;
		capacity++;
	}
	return nextID;
}
