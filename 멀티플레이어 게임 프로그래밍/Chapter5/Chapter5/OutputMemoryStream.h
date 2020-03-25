#pragma once
#include <iostream>
#include <unordered_map>
#include "MemoryStream.h"
#include "Affine.h"
using namespace std;

class OutputMemoryStream: public MemoryStream {
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
	void Write(const unordered_map<int, int>& inData);
	void Write(const Affine& inData);
	virtual void Serialize(void* ioData, uint32_t inByteCount) {
		Write(ioData, inByteCount);
	}

	virtual bool IsInput() const { return false; }

private:
	void ReallocBuffer(uint32_t inNewLength);
};