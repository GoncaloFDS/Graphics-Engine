#include "AnimationManager.h"

AnimationManager::AnimationManager()
{
	this->animations = new std::map<SceneNode*, std::vector<Animation*>>();
}

AnimationManager::~AnimationManager()
{
	for (auto it = this->animations->begin(); it != this->animations->end(); ++it) {
		for (auto animation : it->second) {
			delete(animation);
		}
	}
	this->animations->clear();
	delete(this->animations);
}

void AnimationManager::add(Animation* animation)
{
	auto node = animation->node;
	if (this->animations->find(node) != this->animations->end()) {
		std::vector<Animation*>& nodeAnims = this->animations->find(node)->second;
		nodeAnims.push_back(animation);
	}
	else {
		std::vector<Animation*> newAnimations = std::vector<Animation*>();
		newAnimations.push_back(animation);
		this->animations->insert(std::pair<SceneNode*, std::vector<Animation*>>(node, newAnimations));
	}
}

void AnimationManager::remove(SceneNode * node, Animation* ani)
{
	
}

Animation* AnimationManager::create(SceneNode* node, Vector3 desired, float duration) {
	Animation* ani = new Animation(node, desired, duration);
	this->add(ani);
	return ani;
}

Animation* AnimationManager::create(SceneNode* node, Quaternion desired, float duration) {
	Animation* ani = new Animation(node, desired, duration);
	this->add(ani);
	return ani;
}

Animation* AnimationManager::create(SceneNode* node, Vector3 pDesired, Quaternion qDesired, float duration) {
	Animation* ani = new Animation(node, pDesired, qDesired, duration);
	this->add(ani);
	return ani;
}

void AnimationManager::play(float deltaTime)
{
	if(this->isActive){
		for (auto it = this->animations->begin(); it != this->animations->end(); ++it) {
			auto& animations = it->second;
			for (auto& animation : animations) {
				if (!animation->isFinished) {
					animation->animate(deltaTime);
					break;
				}
			}
		}
	}
}

void AnimationManager::rewind(float deltaTime)
{
	if(this->isActive){
		for (auto it = this->animations->rbegin(); it != this->animations->rend(); ++it) {
			auto& nodeAnimations = it->second;
			for (int i = nodeAnimations.size() - 1; i >= 0; i--) {
				if (nodeAnimations[i]->isRewindable) {
					nodeAnimations[i]->rewind(deltaTime);
					break;
				}
			}
		}
	}
}

void AnimationManager::toggle()
{
	this->isActive = !this->isActive;
}

void AnimationManager::toggleRewind()
{
	this->isPlaying = !isPlaying;
}

