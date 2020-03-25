#pragma once
#include <unordered_map>
#include "GameObject.h"

using GameObjectCreationFunc = GameObject* (*)();

class ObjectCreationRegistry {
private:
	ObjectCreationRegistry() {}
	std::unordered_map<uint32_t, GameObjectCreationFunc> nameToGameObjectCreationFunctionMap;

public:
	static ObjectCreationRegistry& Get() {
		static ObjectCreationRegistry instance;
		return instance;
	}

	template<class T>
	void RegisterCreationFunction() {
		assert(nameToGameObjectCreationFunctionMap.find(T::kClassId) ==
			nameToGameObjectCreationFunctionMap.end());
		nameToGameObjectCreationFunctionMap[T::kClassId] = T::CreateInstance;
	}

	GameObject* CreateGameObject(uint32_t inClassId) {
		GameObjectCreationFunc creationFunc = nameToGameObjectCreationFunctionMap[inClassId];
		GameObject* gameObject = creationFunc();
		return gameObject;
	}
};
