#pragma once
#include "GameObject.h"

class RoboCheese :public GameObject {
public:
	virtual void Update();

	CLASS_IDENTIFICATION('RBCH', RoboCheese)
};