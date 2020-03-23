#pragma once
#include <iostream>
#include <vector>
#include <initializer_list>
using namespace std;

enum EPrimitiveType {
	EPT_Int,
	EPT_String,
	EPT_Float
};

class MemberVariable {
private:
	std::string name;
	EPrimitiveType primitiveType;
	uint32_t offset;

public:
	MemberVariable(const char* inName, EPrimitiveType inPrimitiveType, uint32_t inOffset) :
		name(inName), primitiveType(inPrimitiveType), offset(inOffset) {}

	EPrimitiveType GetPrimitiveType() const { return primitiveType; }
	uint32_t GetOffset() const { return offset; }
};

class DataType {
private:
	std::vector<MemberVariable> memberVariables;

public:
	DataType(std::initializer_list<MemberVariable> inMVs) :memberVariables(inMVs) {}
	
	const std::vector<MemberVariable>& GetMemberVariables() const { return memberVariables; }
};