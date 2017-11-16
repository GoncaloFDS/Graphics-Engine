#pragma once
#include <vector>
#include "Animation.h"

class Animator {
	std::vector<Animation*> Animations;
	int CurrentIdx = 0;
	bool IsActive = false;
	bool IsInverted = false;

public:
	Animator() = default;
	void addAnimation(Animation* a);
	void play(float deltaTime);
	void toggle();
	void reverse();

};