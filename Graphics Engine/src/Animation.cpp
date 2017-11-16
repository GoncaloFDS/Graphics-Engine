#include "Animation.h"
#include "Quat.h"

Animation::Animation(std::vector<SceneNode*> ns, std::vector<NodeState> start, 
	std::vector<NodeState> end, float duration) {
	Nodes = ns;
	StartStates = start;
	EndStates = end;
	Duration = duration;
}

void Animation::play(const float deltaTime) {
	if(IsActive && currentTime < Duration) {
		for(int i = 0; i < Nodes.size(); i++) {
			Vec3 t = (deltaTime / Duration) * 
				(EndStates[i].position - StartStates[i].position);
			Nodes[i]->applyTranslation(t);

			Quat lerped = Quat::Lerp(StartStates[i].quat, EndStates[i].quat, 1 - currentTime / Duration).Normalize();

			Nodes[i]->applyRotation(lerped);
			
		}
		currentTime += deltaTime;
	}
	if (currentTime > Duration) {
		if(Next != nullptr)
			Next->start();
	}
}

bool Animation::isActive() {
	return IsActive;
}

void Animation::start() {
	IsActive = true;
}

void Animation::stop() {
	IsActive = false;
}

void Animation::reverse() {
	currentTime = Duration - currentTime;
	const std::vector<NodeState> tempState = StartStates;
	StartStates = EndStates;
	EndStates = tempState;
	Animation* tempAnim = Previous;
	Previous = Next;
	Next = tempAnim;
}

void Animation::setPreviousAnimation(Animation* anim) {
	Previous = anim;
}

void Animation::setNextAnimation(Animation* anim) {
	Next = anim;
}

bool Animation::hasEnded() const {
	return Next == nullptr;
}
