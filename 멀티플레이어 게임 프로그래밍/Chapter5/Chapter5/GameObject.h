#pragma once
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"
#define CLASS_IDENTIFICATION(inCode, inClass)\
enum { kClassId = inCode }; \
virtual uint32_t GetClassId() const { return kClassId; } \
static GameObject* CreateInstance() { return new inClass(); }

class GameObject {
public:
	virtual void Update();
	virtual void Read(InputMemoryBitStream& inStream);
	virtual void Write(OutputMemoryBitStream& inStream);
	virtual void Destroy();

	CLASS_IDENTIFICATION('GOBJ', GameObject)
};