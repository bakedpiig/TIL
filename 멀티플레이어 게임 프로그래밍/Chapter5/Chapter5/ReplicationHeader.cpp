#include "ReplicationHeader.h"
#include "BitCounter.h"

ReplicationHeader::ReplicationHeader(ReplicationAction inRA, uint32_t inNetworkId, uint32_t inClassId = 0)
	:replicationAction(inRA), networkId(inNetworkId), classId(inClassId) {}

void ReplicationHeader::Write(OutputMemoryBitStream& inStream) {
	inStream.WriteBits(replicationAction, GetRequiredBits(RA_MAX));
	inStream.Write(networkId);

	if (replicationAction != RA_Destroy)
		inStream.Write(classId);
}

void ReplicationHeader::Read(InputMemoryBitStream& inStream) {
	inStream.Read(replicationAction, GetRequiredBits(RA_MAX));
	inStream.Read(networkId);

	if (replicationAction != RA_Destroy)
		inStream.Read(classId);
}