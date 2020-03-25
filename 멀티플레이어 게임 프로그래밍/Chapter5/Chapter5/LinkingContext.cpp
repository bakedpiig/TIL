#include "LinkingContext.h"

uint32_t LinkingContext::GetNetworkId(GameObject* inGameObject, bool inShouldCreateIfNotFound) {
	auto it = gameObjectToNetworkIdMap.find(inGameObject);
	if (it != gameObjectToNetworkIdMap.end())
		return it->second;
	else if (inShouldCreateIfNotFound) {
		uint32_t newNetworkId = nextNetworkId++;
		AddGameObject(inGameObject, newNetworkId);
		return newNetworkId;
	}
	return 0;
}

GameObject* LinkingContext::GetGameObject(uint32_t inNetworkId) {
	auto it = networkIdToGameObjectMap.find(inNetworkId);
	if (it != networkIdToGameObjectMap.end())
		return it->second;
	return nullptr;
}

void LinkingContext::AddGameObject(GameObject* inGameObject, uint32_t inNetworkId) {
	networkIdToGameObjectMap[inNetworkId] = inGameObject;
	gameObjectToNetworkIdMap[inGameObject] = inNetworkId;
}

void LinkingContext::RemoveGameObject(GameObject* inGameObject) {
	uint32_t networkId = gameObjectToNetworkIdMap[inGameObject];
	gameObjectToNetworkIdMap.erase(inGameObject);
	networkIdToGameObjectMap.erase(networkId);
}