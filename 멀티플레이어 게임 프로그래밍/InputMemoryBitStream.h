#pragma once
#include <iostream>
#include <vector>
#include "LinkingContext.h"
#include "GameObject.h"
using namespace std;

class InputMemoryBitStream {
private:
	char* buffer;
	int32_t bitHead;
	int32_t bitCapacity;

	LinkingContext* linkingContext;
public:
	void Read(uint32_t);

	template<typename T>
	void Read(vector<T>& outVector) {
		size_t elementCount;
		Read(elementCount);
		outVector.resize(elementCount);
		for (T& element : outVector)
			Read(element);
	}

	void Read(GameObject*& outGameObject) {
		uint32_t networkId;
		Read(networkId);
		outGameObject = linkingContext->GetGameObject(networkId);
	}
};