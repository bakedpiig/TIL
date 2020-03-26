#include "LinkingContext.h"
#include "InputMemoryBitStream.h"
#include "OutputMemoryStream.h"
#include "RoboCat.h"
#include "RoboMouse.h"
#include "RoboCheese.h"
#include "ObjectCreationRegistry.h"
#include "RelectionSystem.h"
#include "RPCManager.h"

void WriteClassType(OutputMemoryStream& inStream, const GameObject* inGameObject) {
	if (dynamic_cast<const RoboCat*>(inGameObject))
		inStream.Write(static_cast<uint32_t>('RBCT'));
	else if (dynamic_cast<const RoboMouse*>(inGameObject))
		inStream.Write(static_cast<uint32_t>('RBMS'));
	else if (dynamic_cast<const RoboCheese*>(inGameObject))
		inStream.Write(static_cast<uint32_t>('RBCH'));
}

void RegisterObjectCreation() {
	ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<RoboCat>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<RoboMouse>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<RoboCheese>();
}

GameObject* CreateGameObjectFromStream(InputMemoryBitStream& inStream) {
	uint32_t classIdentifier;
	inStream.Read(classIdentifier);
	switch (classIdentifier) {
	case 'RBCT':
		return new RoboCat();
	case 'RBMS':
		return new RoboMouse();
	case 'RBCH':
		return new RoboCheese();
	}
	return nullptr;
}

void Serialize(MemoryStream* inStream, const DataType* inDataType, uint8_t* inData, uint32_t inProperties) {
	inStream->Serialize(inProperties);

	const auto& mvs = inDataType->GetMemberVariables();
	for (int mvIndex = 0, c = mvs.size(); mvIndex < c; mvIndex++) {
		if (((1 << mvIndex) & inProperties) != 0) {
			const auto& mv = mvs[mvIndex];
			void* mvData = inData + mv.GetOffset();
			switch (mv.GetPrimitiveType()) {
			case EPT_Int:
				inStream->Serialize(*reinterpret_cast<int*>(mvData));
				break;
			case EPT_String:
				inStream->Serialize(*reinterpret_cast<string*>(mvData));
				break;
			case EPT_Float:
				inStream->Serialize(*reinterpret_cast<float*>(mvData));
				break;
			}
		}
	}
}

void PlaySound(string name, Vector3 location, float volume);

void UnwrapPlaySound(InputMemoryBitStream& inStream) {
	string soundName;
	Vector3 location;
	float volume;

	inStream.Read(soundName);
	inStream.Read(location);
	inStream.Read(volume);
	PlaySound(soundName, location, volume);
}

void RegisterRPCs(RPCManager* inRPCManager) {
	inRPCManager->RegisterUnwrapFunction('PSND', UnwrapPlaySound);
}