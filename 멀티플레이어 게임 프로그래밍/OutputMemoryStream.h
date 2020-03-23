#pragma once
#include <iostream>
using namespace std;

class OutputMemoryStream {
private:
	char* buffer;
	uint32_t head;
	uint32_t capacity;

public:
	OutputMemoryStream() :
		buffer(nullptr), head(0), capacity(0) {
		ReallocBuffer(32);
	}
	~OutputMemoryStream() { free(buffer); }

	const char* GetBufferPtr() const { return buffer; }
	uint32_t GetLength() const { return head; }

	void Write(const void* inData, size_t inByteCount);
	void Write(uint32_t inData) { Write(&inData, sizeof(inData)); }
	void Write(int32_t inData) { Write(&inData, sizeof(inData)); }
	template<typename T> void Write(const T& inData);

private:
	void ReallocBuffer(uint32_t inNewLength);
};