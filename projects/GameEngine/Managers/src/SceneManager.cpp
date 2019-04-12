#include "SceneManager.h"


using namespace Managers;

Managers::SceneManager::SceneManager()
{
	this->sceneMap = new std::map<std::string, SceneGraph*>();
}

Managers::SceneManager::~SceneManager()
{
	for (auto it = this->sceneMap->begin(); it != this->sceneMap->end(); ++it) {
		it->second->Release();
	}
	sceneMap->clear();
	delete sceneMap;
}

SceneGraph * Managers::SceneManager::create(std::string sceneName)
{
	SceneGraph * scene = new SceneGraph();
	(*sceneMap)[sceneName] = scene;
	return scene;
}

void Managers::SceneManager::add(std::string sceneName, SceneGraph * scene)
{
	(*sceneMap)[sceneName] = scene;
}

void Managers::SceneManager::remove(std::string sceneName)
{
	this->sceneMap->erase(sceneName);
}

SceneGraph * Managers::SceneManager::get(std::string sceneName)
{
	auto found = this->sceneMap->find(sceneName);
	if (found != this->sceneMap->end()) {
		return found->second;
	}
	return nullptr;
}
