#include "InputMemoryStream.h"

void InputMemoryStream::Read(void* outData, uint32_t inByteCount) {
	uint32_t resultHead = head + inByteCount;
	memcpy(outData, buffer + head, inByteCount);
	head = resultHead;
}