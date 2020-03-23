#include "OutputMemoryStream.h"

void OutputMemoryStream::ReallocBuffer(uint32_t inNewLength) {
	buffer = static_cast<char*>(std::realloc(buffer, inNewLength));
	if (!buffer) {
		//TODO: Failed Realloc
	}
	capacity = inNewLength;
}

void OutputMemoryStream::Write(const void* inData, size_t inByteCount) {
	uint32_t resultHead = head + static_cast<uint32_t>(inByteCount);
	if (resultHead > capacity)
		ReallocBuffer(capacity * 2 > resultHead ? capacity * 2 : resultHead);
	std::memcpy(buffer + head, inData, inByteCount);
	head = resultHead;
}

template<typename T>
void OutputMemoryStream::Write(const T& inData) {
	static_assert(std::is_arithmetic<T>::value ||
		std::is_enum<T>::value,
		"Generic Write only supports primitive data types");

	Write(&inData, sizeof(inData));
}