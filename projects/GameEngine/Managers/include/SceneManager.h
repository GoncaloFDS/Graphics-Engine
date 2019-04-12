#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "IManager.h"
#include "SceneGraph.h"
#include <map>

namespace Managers {
	class SceneManager : public IManager<SceneManager>
	{
	//	SceneManager * instance;
		friend IManager<SceneManager>;
		SceneManager();
		~SceneManager();
		std::map<std::string, SceneGraph*>* sceneMap;
		public:
			SceneGraph* create(std::string sceneName);
			void add(std::string sceneName, SceneGraph* scene);
			void remove(std::string sceneName);
			SceneGraph* get(std::string sceneName);
	};
}
#endif
