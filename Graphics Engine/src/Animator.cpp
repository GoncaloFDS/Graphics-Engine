#include "Animator.h";

void Animator::addAnimation(Animation* a) {
	Animations.push_back(a);
}

void Animator::play(const float deltaTime) {
	if(IsActive && Animations[CurrentIdx]->play(deltaTime)) {
		if (IsInverted && CurrentIdx != 0)
			CurrentIdx--;
		else if(!IsInverted && CurrentIdx < Animations.size()-1)
			CurrentIdx++;
	}
}

void Animator::toggle() {
	IsActive = !IsActive;
}

void Animator::reverse() {
	
		IsInverted = !IsInverted;
		for (Animation* a : Animations)
			a->reverse();
	
}
