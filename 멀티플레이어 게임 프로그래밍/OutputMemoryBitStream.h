#pragma once
#pragma once
#include <iostream>
#include <vector>
#include "LinkingContext.h"
#include "GameObject.h"
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
	//TODO: 엔디언 호환성

private:
	void ReallocBuffer(uint32_t inNewBitCapacity);
};