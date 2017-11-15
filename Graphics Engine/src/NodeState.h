#pragma once
#include "Vec3.h"
#include "Quat.h"

struct NodeState {
	Vec3 position = Vec3(0, 0, 0);
	Quat quat = Quat(0, Vec4(0, 0, 1, 0));

	NodeState(Vec3 pos, Quat q) :
		position(pos), quat(q) {}
};
