#pragma once
#define CLASS_IDENTIFICATION(inCode, inClass)\
enum { kClassId = inCode }; \
virtual uint32_t GetClassId() const { return kClassId; } \
static GameObject* CreateInstance() { return new inClass(); }

class GameObject {
public:
	virtual void Update();

	CLASS_IDENTIFICATION('GOBJ', GameObject)
};