#pragma once
#include "GameObject.h"
class RoboMouse :public GameObject {
public:
	virtual void Update();

	CLASS_IDENTIFICATION('RBMS', RoboMouse)
};