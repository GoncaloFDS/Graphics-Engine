#include "Animation.h"
#include "Quat.h"

Animation::Animation(std::vector<SceneNode*> ns, std::vector<NodeState> start, 
	std::vector<NodeState> end, float duration) {
	Nodes = ns;
	StartStates = start;
	EndStates = end;
	Duration = duration;
}

//returns true if animatin has ended
bool Animation::play(const float deltaTime) {
	if(CurrentTime < Duration) {
		for(int i = 0; i < Nodes.size(); i++) {
			Vec3 t = (deltaTime / Duration) * (EndStates[i].position - StartStates[i].position);
			Nodes[i]->applyTranslation(t);
			Quat start = StartStates[i].quat;
			Quat target = EndStates[i].quat;
			if(start != target)
				Nodes[i]->applyRotation(Quat::slerp(start, target, CurrentTime / Duration).normalize());
			
		}
		CurrentTime += deltaTime;
		return false;
	}
	return true;
}

void Animation::reverse() {
	CurrentTime = Duration - CurrentTime;
	const std::vector<NodeState> tempState = StartStates;
	StartStates = EndStates;
	EndStates = tempState;
}
