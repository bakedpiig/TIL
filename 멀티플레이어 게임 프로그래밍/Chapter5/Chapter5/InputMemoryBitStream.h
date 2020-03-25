#pragma once
#include <iostream>
#include <vector>
#include "LinkingContext.h"
#include "GameObject.h"
#include "Quaternion.h"
using namespace std;

class InputMemoryBitStream {
private:
	char* buffer;
	int32_t bitHead;
	int32_t bitCapacity;

	LinkingContext* linkingContext;
public:
	void Read(uint32_t);
	void Read(uint32_t, uint32_t);

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

	void Read(Quaternion& outQuat) {
		float precision = (2.0f / 65535.0f);
		uint32_t f = 0;

		Read(f, 16);
		outQuat.x = ConvertFromFixed(f, -1.0f, precision);
		Read(f, 16);
		outQuat.y = ConvertFromFixed(f, -1.0f, precision);
		Read(f, 16);
		outQuat.z = ConvertFromFixed(f, -1.0f, precision);

		outQuat.w = sqrtf(1.f - (outQuat.x * outQuat.x + outQuat.y * outQuat.y + outQuat.z * outQuat.z));

		bool isNegative;
		Read(isNegative);

		if (isNegative)
			outQuat.w *= -1;
	}

	uint32_t GetRemainingBitCount() {
		return bitCapacity - bitHead;
	}
private:
	uint32_t ConvertToFixed(float inNumber, float inMin, float inPrecision) {
		return static_cast<uint32_t>((inNumber - inMin) / inPrecision);
	}

	uint32_t ConvertFromFixed(uint32_t inNumber, float inMin, float inPrecision) {
		return static_cast<float>(inNumber)* inPrecision + inMin;
	}
};