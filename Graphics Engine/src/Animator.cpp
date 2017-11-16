#include "Animator.h";

void Animator::addAnimation(Animation* a) {
	Animations.push_back(a);
}

void Animator::play(float deltaTime) {
	Animations[CurrentIdx]->play(deltaTime);
}

void Animator::start() {
}

void Animator::stop() {
}

void Animator::reverse() {
}
