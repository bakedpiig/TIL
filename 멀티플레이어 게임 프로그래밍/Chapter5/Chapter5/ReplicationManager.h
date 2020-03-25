#pragma once
#include <unordered_set>
#include "LinkingContext.h"
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"
using namespace std;

class ReplicationManager {
public:
	void ReplicateWorldState(OutputMemoryBitStream& inStream, const std::vector<GameObject*>& inAllObjects);
	void ReceiveWorld(InputMemoryBitStream& inStream);

private:
	void ReplicateIntoStream(OutputMemoryBitStream& inStream, GameObject* inGameObject);
	GameObject* ReceiveReplicatedObject(InputMemoryBitStream& inStream);

	LinkingContext* linkingContext;
	unordered_set<GameObject*> objectsReplicatedToMe;
};