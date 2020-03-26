#include "RPCManager.h"
#include <assert.h>

inline void RPCManager::RegisterUnwrapFunction(uint32_t inName, RPCUnwrapFunc inFunc) {
	assert(nameToRPCTable.find(inName) == nameToRPCTable.end());
	nameToRPCTable[inName] = inFunc;
}

inline void RPCManager::ProcessRPC(InputMemoryBitStream& inStream) {
	uint32_t name;
	inStream.Read(name);
	nameToRPCTable[name](inStream);
}