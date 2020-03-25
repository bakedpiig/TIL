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

void OutputMemoryStream::Write(const unordered_map<int, int>& inData) {
	uint32_t mapSize = inData.size;
	Write(mapSize);
	for (auto d : inData) {
		Write(d.first);
		Write(d.second);
	}
}

void OutputMemoryStream::Write(const Affine& inData) {
	if (inData.matrix[0][0] == 1)
		Write(true);
	else {
		Write(false);
		Write(inData.matrix[0][0]);
	}

	for (int i = 0; i < 3; i++) {
		for (int j = i + 1; j < 4; j++) {
			Write(inData.matrix[i][j]);
		}
	}

	if (inData.matrix[3][3])
		Write(true);
	else
		Write(false);
}