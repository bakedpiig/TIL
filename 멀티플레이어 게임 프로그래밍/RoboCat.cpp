#include "RoboCat.h"

void RoboCat::Write(OutputMemoryStream& inStream) const {
	inStream.Write(health);
	inStream.Write(meowCount);
	//TODO: homeBase
	inStream.Write(name, 128);
	//TODO: miceIndices
}

void RoboCat::Read(InputMemoryStream& inStream) {
	inStream.Read(health);
	inStream.Read(meowCount);
	//TODO: homeBase
	inStream.Read(name, 128);
	//TODO: miceIndices
}