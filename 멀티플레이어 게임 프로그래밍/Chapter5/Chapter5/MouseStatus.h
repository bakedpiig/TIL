#pragma once
#include "RelectionSystem.h"
#include "MemoryStream.h"
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"

#define OffsetOf(c, mv) ((size_t) & (static_cast<c*>(nullptr)->mv))

enum MouseStatusProperties {
	MSP_Name = 1 << 0,
	MSP_LegCount = 1 << 1,
	MSP_HeadCount = 1 << 2,
	MSP_Health = 1 << 3,
	MSP_MAX
};

class MouseStatus {
public:
	std::string name;
	int legCount, headCount;
	float health;

	static DataType* dataType;
	static void InitDataType();

	void Serialize(MemoryStream* inMemoryStream, const DataType* inDataType, uint8_t* inData);

	void Write(OutputMemoryBitStream& inStream, uint32_t inProperties);
	void Read(InputMemoryBitStream& inStream);
};