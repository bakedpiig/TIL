#pragma once
#include <unordered_set>
#include "LinkingContext.h"
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"
#include "RPCManager.h"
using namespace std;

class ReplicationManager {
public:
	void ReplicateWorldState(OutputMemoryBitStream& inStream, const std::vector<GameObject*>& inAllObjects);
	void ReceiveWorld(InputMemoryBitStream& inStream);

	void ReplicateCreate(OutputMemoryBitStream& inStream, GameObject* inGameObject);
	void ReplicateUpdate(OutputMemoryBitStream& inStream, GameObject* inGameObject);
	void ReplicateDestroy(OutputMemoryBitStream& inStream, GameObject* inGameObject);
	void ProcessReplicationAction(InputMemoryBitStream& inStream);

private:
	void ReplicateIntoStream(OutputMemoryBitStream& inStream, GameObject* inGameObject);
	GameObject* ReceiveReplicatedObject(InputMemoryBitStream& inStream);

	LinkingContext* linkingContext;
	unordered_set<GameObject*> objectsReplicatedToMe;
	RPCManager* rpcManager;
};