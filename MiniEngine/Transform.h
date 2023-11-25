#pragma once
#include "Quaternion.h"
#include "Vector3.h"

class Transform {
public:
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	Transform() : position({0, 0, 0}), rotation({1, 0, 0, 0}), scale({1, 1, 1}) { }
};

