#include "InputMemoryStream.h"

void InputMemoryStream::Read(void* outData, uint32_t inByteCount) {
	uint32_t resultHead = head + inByteCount;
	memcpy(outData, buffer + head, inByteCount);
	head = resultHead;
}

void InputMemoryStream::Read(unordered_map<int, int>& outData) {
	uint32_t mapSize;
	Read(&mapSize, sizeof(mapSize));
	for (int i = 0; i < mapSize; i++) {
		uint32_t key;
		Read(&key, sizeof(key));
		uint32_t value;
		Read(&value, sizeof(value));
		outData[key] = value;
	}
}

template<typename T>
void InputMemoryStream::Read(T& outData) {
	static_assert(std::is_arithmetic<T>::value ||
		std::is_enum<T>::value,
		"Generic Write only supports primitive data types");
	uint32_t resultHead = head + sizeof(outData);
	memcpy(outData, buffer + head, sizeof(outData));
	head = resultHead;
}

template<typename tKey,typename tValue>
void InputMemoryStream::Read(unordered_map<tKey, tValue>& outData) {
	uint32_t mapSize;
	Read(&mapSize, sizeof(mapSize));
	for (int i = 0; i < mapSize; i++) {
		tKey key;
		Read(&key);
		tValue value;
		Read(&value);
		outData[key] = value;
	}
}

void InputMemoryStream::Read(Affine& outData) {
	bool isScaleOne;
	Read(isScaleOne);
	if (isScaleOne) {
		outData.matrix[0][0] = 1;
		outData.matrix[1][1] = 1;
		outData.matrix[2][2] = 1;
	}
	else {
		float scale;
		Read(scale);
		outData.matrix[0][0] = scale;
		outData.matrix[1][1] = scale;
		outData.matrix[2][2] = scale;
	}

	for (int i = 0; i < 3; i++) {
		for (int j = i + 1; j < 4; j++) {
			Read(outData.matrix[i][j]);
		}
	}

	bool isVector;
	Read(isVector);
	if (isVector)
		outData.matrix[3][3] = 1;
	else
		outData.matrix[3][3] = 0;
}