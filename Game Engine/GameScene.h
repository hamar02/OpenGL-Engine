#pragma once
#include <vector>
#include "GameObject.h"
#include "AssetManager.h"

class GameScene
{
public:
	GameScene();
	GameScene(AssetManager *otherAssetManager);
	~GameScene();

	int assetID;
	AssetManager *assetManager;


	std::vector<GameObject*> gameObjects;
	GameObject* mainCamera;
	void addEmptyGameObject();
	void addEmptyGameObject(glm::vec3 otherPosition);
	void addGameObject(GameObject* other);

private:

};