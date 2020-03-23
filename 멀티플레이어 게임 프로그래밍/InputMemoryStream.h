#pragma once
#include <iostream>

class InputMemoryStream {
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
};