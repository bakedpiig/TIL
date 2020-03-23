#pragma once
#include <iostream>
#include "MemoryStream.h"

class InputMemoryStream: public MemoryStream {
private:
	char* buffer;
	uint32_t head;
	uint32_t capacity;

public:
	InputMemoryStream(char* inBuffer, uint32_t inByteCount)
		:buffer(inBuffer), capacity(inByteCount), head(0) {}
	~InputMemoryStream() { free(buffer); }

	uint32_t GetRemainingDataSize() const { return capacity - head; }

	void Read(void* outData, uint32_t inByteCount);
	void Read(uint32_t& outData) { Read(&outData, sizeof(outData)); }
	void Read(int32_t& outData) { Read(&outData, sizeof(outData)); }

	virtual void Serialize(void* ioData, uint32_t inByteCount) {
		Read(ioData, inByteCount);
	}

	virtual bool IsInput() const { return true; }
};