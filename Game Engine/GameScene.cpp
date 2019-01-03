#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::GameScene(AssetManager * otherAssetManager)
{
	assetManager = otherAssetManager;
	assetID = assetManager->createNewAsset();
}

GameScene::~GameScene()
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
}

void GameScene::addEmptyGameObject()
{
	GameObject* newEmptyGameObject =new GameObject();
	gameObjects.push_back(newEmptyGameObject);

}

void GameScene::addEmptyGameObject(glm::vec3 otherPosition)
{
	GameObject* newEmptyGameObject = new GameObject(otherPosition);
	gameObjects.push_back(newEmptyGameObject);

}

void GameScene::addGameObject(GameObject* other)
{
}
