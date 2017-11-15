#pragma once
#include <vector>
#include "SceneNode.h";

class Animation {
	std::vector<SceneNode*> Nodes;
	std::vector<NodeState> StartStates;
	std::vector<NodeState> EndStates;
	bool IsActive;
	bool HasEnded;
	float Duration;
	float currentTime;

public:
	Animation(std::vector<SceneNode*> ns, std::vector<NodeState> start, 
		std::vector<NodeState> end, float duration);
	void play(float deltaTime);
	void start();
	void stop();
	void reverse();
};


