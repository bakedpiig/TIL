#include "MouseStatus.h"

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