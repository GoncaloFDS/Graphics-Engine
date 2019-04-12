#pragma once
#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H
#include <map>
#include "IManager.h"
#include <vector>
#include "SceneNode.h"
#include "Animation.h"
using namespace Managers;
class AnimationManager : public IManager<AnimationManager>
{
	//AnimationState ani = AnimationState::Stopped;
	friend IManager <AnimationManager>;
	AnimationManager();
	~AnimationManager();
	std::map<SceneNode*, std::vector<Animation*>>* animations;
	bool isActive = false;
	bool isPlaying = true;
public:
	void add(Animation*);
	void remove(SceneNode*, Animation*);
	Animation * create(SceneNode * node, Vector3 desired, float duration);
	Animation * create(SceneNode * node, Quaternion desired, float duration);
	Animation * create(SceneNode * node, Vector3 pDesired, Quaternion qDesired, float duration);
	void play(float deltaTime);
	void rewind(float deltaTime);
	void toggle();
	void toggleRewind();

};
#endif
