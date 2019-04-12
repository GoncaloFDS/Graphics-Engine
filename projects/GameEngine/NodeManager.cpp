#include "NodeManager.h"

using namespace Managers;

Managers::NodeManager::NodeManager()
{
	this->nodeMap = new std::map<std::string, SceneNode*>();
}

Managers::NodeManager::~NodeManager()
{
	for (auto it = this->nodeMap->begin(); it != this->nodeMap->end(); ++it) {
		it->second->Release();
	}
	nodeMap->clear();
	delete nodeMap;
}

SceneNode * Managers::NodeManager::create(std::string nodeName)
{
	SceneNode * node = new SceneNode();
	(*nodeMap)[nodeName] = node;
	return node;
}

void Managers::NodeManager::add(std::string nodeName, SceneNode* node)
{
	(*nodeMap)[nodeName] = node;
}

void Managers::NodeManager::remove(std::string nodeName)
{
	this->nodeMap->erase(nodeName);
}

SceneNode* Managers::NodeManager::get(std::string nodeName)
{
	auto found = this->nodeMap->find(nodeName);
	if (found != this->nodeMap->end()) {
		return found->second;
	}
	return nullptr;
}
