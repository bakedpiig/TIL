#pragma once
#pragma once
#include <iostream>
using namespace std;

class OutputMemoryBitStream {
private:
	char* buffer;
	uint32_t bitHead;
	uint32_t bitCapacity;

public:
	OutputMemoryBitStream() { ReallocBuffer(256); }
	~OutputMemoryBitStream() { free(buffer); }

	void WriteBits(uint8_t inData, size_t inBitCount);
	void WriteBits(const void* inData, size_t inBitCount);

	const char* GetBufferPtr() const { return buffer; }
	uint32_t GetBitLength() const { return bitHead; }
	uint32_t GetByteLength() const { return (bitHead + 7) >> 3; }

	void WriteBytes(const void* inData, size_t inByteCount) { WriteBits(inData, inByteCount << 3); }

	//TODO: 엔디언 호환성

private:
	void ReallocBuffer(uint32_t inNewBitCapacity);
};