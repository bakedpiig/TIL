#pragma once
#include "InputMemoryBitStream.h"
#include "OutputMemoryBitStream.h"

using RPCUnwrapFunc = void(*)(InputMemoryBitStream&);

class RPCManager {
public:
	void RegisterUnwrapFunction(uint32_t inName, RPCUnwrapFunc inFunc);
	void ProcessRPC(InputMemoryBitStream& inStream);

	std::unordered_map<uint32_t, RPCUnwrapFunc> nameToRPCTable;
};