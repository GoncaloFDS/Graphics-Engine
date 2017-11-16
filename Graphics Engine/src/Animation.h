#pragma once
#include <vector>
#include "SceneNode.h"

class Animation {
	std::vector<SceneNode*> Nodes;
	std::vector<NodeState> StartStates;
	std::vector<NodeState> EndStates;
	float Duration;
	float CurrentTime;

public:
	Animation(std::vector<SceneNode*> ns, std::vector<NodeState> start, 
		std::vector<NodeState> end, float duration);
	bool play(float deltaTime);
	void reverse();

};


