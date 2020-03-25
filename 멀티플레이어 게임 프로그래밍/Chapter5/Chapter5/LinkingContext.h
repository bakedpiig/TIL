#pragma once
#include <iostream>
#include <unordered_map>
#include "GameObject.h"

class LinkingContext {
private:
	uint32_t nextNetworkId;
	std::unordered_map<uint32_t, GameObject*> networkIdToGameObjectMap;
	std::unordered_map<GameObject*, uint32_t> gameObjectToNetworkIdMap;
public:
	LinkingContext() :nextNetworkId(1) {}

	uint32_t GetNetworkId(GameObject* inGameObject, bool inShouldCreateIfNotFound);
	GameObject* GetGameObject(uint32_t inNetworkId);

	void AddGameObject(GameObject* inGameObject, uint32_t inNetworkId);
	void RemoveGameObject(GameObject* inGameObject);
};