#include "ReplicationManager.h"
#include "PacketType.h"
#include "BitCounter.h"
#include "ObjectCreationRegistry.h"

void ReplicationManager::ReplicateIntoStream(OutputMemoryBitStream& inStream, GameObject* inGameObject) {
	inStream.Write(linkingContext->GetNetworkId(inGameObject, true));
	inStream.Write(inGameObject->GetClassId());
	inGameObject->Write(inStream);
}

void ReplicationManager::ReplicateWorldState(OutputMemoryBitStream& inStream, const std::vector<GameObject*>& inAllObjects) {
	inStream.WriteBits(PT_ReplicationData, GetRequiredBits(PT_MAX));
	for (GameObject* go : inAllObjects)
		ReplicateIntoStream(inStream, go);
}

void ReplicationManager::ReceiveWorld(InputMemoryBitStream& inStream) {
	unordered_set<GameObject*> receivedObjects;

	while (inStream.GetRemainingBitCount() > 0) {
		GameObject* receivedGameObject = ReceiveReplicatedObject(inStream);
		receivedObjects.insert(receivedGameObject);
	}

	for (GameObject* go : objectsReplicatedToMe) {
		if (receivedObjects.find(go) == receivedObjects.end()) {
			linkingContext->RemoveGameObject(go);
			go->Destroy();
		}
	}

	objectsReplicatedToMe = receivedObjects;
}

GameObject* ReplicationManager::ReceiveReplicatedObject(InputMemoryBitStream& inStream) {
	uint32_t networkId;
	uint32_t classId;
	inStream.Read(networkId);
	inStream.Read(classId);

	GameObject* go = linkingContext->GetGameObject(networkId);
	if (!go) {
		go = ObjectCreationRegistry::Get().CreateGameObject(classId);
		linkingContext->AddGameObject(go, networkId);
	}

	go->Read(inStream);
	return go;
}