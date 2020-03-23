#pragma once
#include "RelectionSystem.h"
#include "MemoryStream.h"
#define OffsetOf(c, mv) ((size_t) & (static_cast<c*>(nullptr)->mv))

class MouseStatus {
public:
	std::string name;
	int legCount, headCount;
	float health;

	static DataType* dataType;
	static void InitDataType() {
		dataType = new DataType({
			MemberVariable("name",EPT_String,OffsetOf(MouseStatus,name)),
			MemberVariable("legCount",EPT_Int,OffsetOf(MouseStatus,legCount)),
			MemberVariable("headCount",EPT_Int,OffsetOf(MouseStatus,headCount)),
			MemberVariable("health",EPT_Float,OffsetOf(MouseStatus,health))
			});
	}

	void Serialize(MemoryStream* inMemoryStream, const DataType* inDataType, uint8_t* inData) {
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
};