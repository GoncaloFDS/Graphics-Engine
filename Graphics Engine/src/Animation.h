#pragma once
#include <vector>
#include "SceneNode.h";

class Animation {
	std::vector<SceneNode*> Nodes;
	std::vector<NodeState> StartStates;
	std::vector<NodeState> EndStates;
	bool IsActive;
	float Duration;
	float currentTime;
	Animation* Previous = nullptr;
	Animation* Next = nullptr;

public:
	Animation(std::vector<SceneNode*> ns, std::vector<NodeState> start, 
		std::vector<NodeState> end, float duration);
	void play(float deltaTime);
	bool isActive();
	void start();
	void stop();
	void reverse();
	void setPreviousAnimation(Animation* anim);
	void setNextAnimation(Animation* anim);
	bool hasEnded() const;
};


