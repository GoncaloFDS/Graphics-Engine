#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H
#include "Quaternion.h"
#include "SceneNode.h"
enum AnimationState {
	Stopped, Rewinding, Animating
};
class Animation
{

public:
	Animation();
	Animation(SceneNode*, Quaternion final, float duration);
	Animation(SceneNode* node, Vector3 final, float duration);
	Animation(SceneNode* node, Vector3 pFinal, Quaternion qFinal, float duration);
	AnimationState state;
	SceneNode* node;
	bool isFinished = false;
	bool isRewindable = false;
	~Animation();
	float duration;
	Quaternion qInitial;
	Quaternion qCurrent;
	Quaternion qFinal;	
	Vector3 pInitial;
	Vector3 pCurrent;
	Vector3 pFinal;
	Vector3 lastState;
	float interpolationFactor;
	void animate(float delta);
	void rewind(float delta);
};

#endif