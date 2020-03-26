#include <iostream>
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"

enum ReplicationAction {
	RA_Create,
	RA_Update,
	RA_Destroy,
	RA_RPC,
	RA_MAX
};

class ReplicationHeader {
public:
	ReplicationHeader() {}
	ReplicationHeader(ReplicationAction inRA, uint32_t inNetworkId, uint32_t inClassId = 0);

	ReplicationAction replicationAction;
	uint32_t networkId;
	uint32_t classId;

	void Write(OutputMemoryBitStream& inStream);
	void Read(InputMemoryBitStream& inStream);
};