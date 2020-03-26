#include "ReplicationManager.h"
#include "PacketType.h"
#include "BitCounter.h"
#include "ObjectCreationRegistry.h"
#include "ReplicationHeader.h"

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

void ReplicationManager::ReplicateCreate(OutputMemoryBitStream& inStream, GameObject* inGameObject) {
	ReplicationHeader rh(RA_Create, linkingContext->GetNetworkId(inGameObject, false), inGameObject->GetClassId());
	rh.Write(inStream);
}

void ReplicationManager::ReplicateUpdate(OutputMemoryBitStream& inStream, GameObject* inGameObject) {
	ReplicationHeader rh(RA_Update, linkingContext->GetNetworkId(inGameObject, false), inGameObject->GetClassId());
	rh.Write(inStream);
}

void ReplicationManager::ReplicateDestroy(OutputMemoryBitStream& inStream, GameObject* inGameObject) {
	ReplicationHeader rh(RA_Destroy, linkingContext->GetNetworkId(inGameObject, false));
	rh.Write(inStream);
}

void ReplicationManager::ProcessReplicationAction(InputMemoryBitStream& inStream) {
	ReplicationHeader rh;
	rh.Read(inStream);
	switch (rh.replicationAction) {
	case RA_Create:
		GameObject* go = ObjectCreationRegistry::Get().CreateGameObject(rh.classId);
		linkingContext->AddGameObject(go, rh.networkId);
		go->Read(inStream);
		break;
	case RA_Update:
		GameObject* go = linkingContext->GetGameObject(rh.networkId);
		if (go)
			go->Read(inStream);
		else {
			uint32_t classId = rh.classId;
			go = ObjectCreationRegistry::Get().CreateGameObject(classId);
			go->Read(inStream);
			delete go;
		}
		break;
	case RA_Destroy:
		GameObject* go = linkingContext->GetGameObject(rh.networkId);
		linkingContext->RemoveGameObject(go);
		go->Destroy();
		break;
	case RA_RPC:
		rpcManager->ProcessRPC(inStream);
		break;
	default:
		break;
	}
}