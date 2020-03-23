#pragma once
#include <iostream>
#include <unordered_map>
#include "GameObject.h"

class LinkingContext {
private:
	std::unordered_map<uint32_t, GameObject*> networkIdToGameObjectMap;
	std::unordered_map<GameObject*, uint32_t> gameObjectToNetworkIdMap;
public:
	uint32_t GetNetworkId(GameObject* inGameObject) {
		auto it = gameObjectToNetworkIdMap.find(inGameObject);
		if (it != gameObjectToNetworkIdMap.end())
			return it->second;
		else
			return 0;
	}

	GameObject* GetGameObject(uint32_t inNetworkId) {
		auto it = networkIdToGameObjectMap.find(inNetworkId);
		if (it != networkIdToGameObjectMap.end())
			return it->second;
		else
			return nullptr;
	}
};