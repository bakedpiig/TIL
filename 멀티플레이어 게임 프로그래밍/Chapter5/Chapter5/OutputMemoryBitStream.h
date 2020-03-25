#pragma once
#pragma once
#include <iostream>
#include <vector>
#include "LinkingContext.h"
#include "GameObject.h"
#include "Quaternion.h"
#include "Vector3.h"
using namespace std;

class OutputMemoryBitStream {
private:
	char* buffer;
	uint32_t bitHead;
	uint32_t bitCapacity;

	LinkingContext* linkingContext;

public:
	OutputMemoryBitStream() { ReallocBuffer(256); }
	~OutputMemoryBitStream() { free(buffer); }

	void WriteBits(uint8_t inData, size_t inBitCount);
	void WriteBits(const void* inData, size_t inBitCount);

	const char* GetBufferPtr() const { return buffer; }
	uint32_t GetBitLength() const { return bitHead; }
	uint32_t GetByteLength() const { return (bitHead + 7) >> 3; }

	void WriteBytes(const void* inData, size_t inByteCount) { WriteBits(inData, inByteCount << 3); }

	void Write(uint32_t inData, size_t inBitCount = sizeof(uint32_t) * 8) { WriteBits(&inData, inBitCount); }
	void Write(int inData, size_t inBitCount = sizeof(int) * 8) { WriteBits(&inData, inBitCount); }
	void Write(float inData) { WriteBits(&inData, sizeof(float) * 8); }
	void Write(uint16_t inData, size_t inBitCount = sizeof(uint16_t) * 8) { WriteBits(&inData, inBitCount); }
	void Write(int16_t inData, size_t inBitCount = sizeof(int16_t) * 8) { WriteBits(&inData, inBitCount); }
	void Write(int8_t inData, size_t inBitCount = sizeof(int8_t) * 8) { WriteBits(&inData, inBitCount); }
	void Write(string inData);
	void Write(bool inData) { WriteBits(&inData, 1); }

	template<typename T>
	void Write(const std::vector<T>& inVector) {
		size_t elementCount = inVector.size();
		Write(elementCount);
		for (const T& element : inVector)
			Write(element);
	}

	void Write(const GameObject* inGameObject) {
		uint32_t networkId = linkingContext->GetNetworkId(const_cast<GameObject*>(inGameObject));
		Write(networkId);
	}

	void WritePos(const Vector3& inVector) {
		Write(inVector.x);
		Write(inVector.z);

		//entropy encoding
		if (inVector.y == 0)		//If it's on bottom
			Write(true);	
		else if (inVector.y == 100) {		//If it's attached to the ceiling
			Write(true);
			Write(false);
		}
		else {
			Write(false);
			Write(inVector.y);
		}
	}

	//Fixed-point compression
	void WritePosFixed(const Vector3& inVector) {
		Write(ConvertToFixed(inVector.x, -2000.0f, 0.1f), 16);
		Write(ConvertToFixed(inVector.z, -2000.0f, 0.1f), 16);

		if (inVector.y == 0)
			Write(true);
		else if (inVector.y == 100) {
			Write(true);
			Write(false);
		}
		else {
			Write(false);
			Write(inVector.y);
		}
	}

	//Geometry compression
	void Write(const Quaternion& inQuat) {
		float precision = (2.0f / 65535.0f);
		Write(ConvertToFixed(inQuat.x, -1.0f, precision), 16);
		Write(ConvertToFixed(inQuat.y, -1.0f, precision), 16);
		Write(ConvertToFixed(inQuat.z, -1.0f, precision), 16);
		Write(inQuat.w < 0);
	}
	//TODO: Big & Small Endian

private:
	void ReallocBuffer(uint32_t inNewBitCapacity);
	uint32_t ConvertToFixed(float inNumber, float inMin, float inPrecision) {
		return static_cast<uint32_t>((inNumber - inMin) / inPrecision);
	}

	uint32_t ConvertFromFixed(uint32_t inNumber, float inMin, float inPrecision) {
		return static_cast<float>(inNumber)* inPrecision + inMin;
	}
};