#include "MouseStatus.h"
#include "BitCounter.h"

void MouseStatus::InitDataType() {
	dataType = new DataType({
		MemberVariable("name",EPT_String,OffsetOf(MouseStatus,name)),
		MemberVariable("legCount",EPT_Int,OffsetOf(MouseStatus,legCount)),
		MemberVariable("headCount",EPT_Int,OffsetOf(MouseStatus,headCount)),
		MemberVariable("health",EPT_Float,OffsetOf(MouseStatus,health))
		});
}

void MouseStatus::Serialize(MemoryStream* inMemoryStream, const DataType* inDataType, uint8_t* inData) {
	for (auto& mv : inDataType->GetMemberVariables()) {
		void* mvData = inData + mv.GetOffset();
		switch (mv.GetPrimitiveType()) {
		case EPT_Int:
			inMemoryStream->Serialize(*(int*)mvData);
			break;
		case EPT_String:
			inMemoryStream->Serialize(*(std::string*)mvData);
			break;
		case EPT_Float:
			inMemoryStream->Serialize(*(float*)mvData);
			break;
		}
	}
}

void MouseStatus::Write(OutputMemoryBitStream& inStream, uint32_t inProperties) {
	inStream.Write(inProperties, GetRequiredBits(MSP_MAX));

	if ((inProperties & MSP_Name) != 0)
		inStream.Write(name);
	if ((inProperties & MSP_LegCount) != 0)
		inStream.Write(legCount);
	if ((inProperties & MSP_HeadCount) != 0)
		inStream.Write(headCount);
	if ((inProperties & MSP_Health) != 0)
		inStream.Write(health);
}

void MouseStatus::Read(InputMemoryBitStream& inStream) {
	uint32_t writtenProperties;
	inStream.Read(writtenProperties, GetRequiredBits(MSP_MAX));

	if ((writtenProperties & MSP_Name) != 0)
		inStream.Read(name);
	if ((writtenProperties & MSP_LegCount) != 0)
		inStream.Read(legCount);
	if ((writtenProperties & MSP_HeadCount) != 0)
		inStream.Read(headCount);
	if ((writtenProperties & MSP_Health) != 0)
		inStream.Read(health);
}