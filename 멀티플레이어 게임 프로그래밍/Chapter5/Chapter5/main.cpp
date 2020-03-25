#include "LinkingContext.h"
#include "InputMemoryBitStream.h"
#include "OutputMemoryStream.h"
#include "RoboCat.h"
#include "RoboMouse.h"
#include "RoboCheese.h"
#include "ObjectCreationRegistry.h"

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
	
	return nullptr;
}