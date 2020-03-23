#include "RoboCat.h"

void RoboCat::Write(OutputMemoryStream& inStream) const {
	inStream.Write(health);
	inStream.Write(meowCount);
	//TODO: homeBase
	uint8_t nameLength = static_cast<uint8_t>(strlen(name));
	inStream.Write(nameLength);
	inStream.Write(name, nameLength);
	//TODO: miceIndices
}

void RoboCat::Read(InputMemoryStream& inStream) {
	inStream.Read(health);
	inStream.Read(meowCount);
	//TODO: homeBase
	inStream.Read(name, 128);
	//TODO: miceIndices
}