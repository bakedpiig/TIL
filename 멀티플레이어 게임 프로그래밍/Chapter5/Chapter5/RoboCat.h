#pragma once
#include "GameObject.h"
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <vector>
using namespace std;

class RoboCat :public GameObject {
private:
	int32_t health;
	int32_t meowCount;
	GameObject* homeBase;
	char name[128];
	vector<int32_t> miceIndices;

	Vector3 position;
	Quaternion rotation;
public:
	RoboCat() :health(10), meowCount(3), homeBase(0) {
		name[0] = '\0';
	}

	virtual void Update();
	
	CLASS_IDENTIFICATION('RBCT', RoboCat)

	void Write(OutputMemoryStream& inStream) const;
	void Read(InputMemoryStream& inStream);
};