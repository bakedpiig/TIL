#include "OutputMemoryBitStream.h"

void OutputMemoryBitStream::WriteBits(uint8_t inData, size_t inBitCount) {
	uint32_t nextBitHead = bitHead + static_cast<uint32_t>(inBitCount);
	if (nextBitHead > bitCapacity)
		ReallocBuffer(bitCapacity * 2 > nextBitHead ? bitCapacity * 2 : nextBitHead);
	
	uint32_t byteOffset = bitHead >> 3;
	uint32_t bitOffset = bitHead & 0x7;

	uint8_t currentMask = ~(0xff << bitOffset);
	buffer[byteOffset] = (buffer[byteOffset] & currentMask) | (inData << bitOffset);

	uint32_t bitsFreeThisByte = 8 - bitOffset;
	if (bitsFreeThisByte < inBitCount)
		buffer[byteOffset + 1] = inData >> bitsFreeThisByte;
	bitHead = nextBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* inData, size_t inBitCount) {
	const char* srcByte = static_cast<const char*>(inData);

	while (inBitCount > 8) {
		WriteBits(*srcByte, 8);
		++srcByte;
		inBitCount -= 8;
	}

	if (inBitCount > 0)
		WriteBits(*srcByte, inBitCount);
}

void OutputMemoryBitStream::ReallocBuffer(uint32_t inNewBitCapacity) {
	int newLength = inNewBitCapacity / 8;
	if (!(inNewBitCapacity % 8))
		newLength++;

	buffer = static_cast<char*>(std::realloc(buffer, newLength));
	if (!buffer) {
		//TODO: Failed Realloc
	}
	bitCapacity = inNewBitCapacity;
}