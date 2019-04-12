#pragma once
#ifndef NODEMANAGER_H
#define NODEMANAGER_H
#include "IManager.h"
#include "SceneNode.h"
#include <map>

namespace Managers {
	class NodeManager : public IManager<NodeManager>
	{
		//	SceneManager * instance;
		friend IManager<NodeManager>;
		NodeManager();
		~NodeManager();
		std::map<std::string, SceneNode*>* nodeMap;
	public:
		SceneNode * create(std::string nodeName);
		void add(std::string nodeName, SceneNode* node);
		void remove(std::string nodeName);
		SceneNode* get(std::string nodeName);
	};
}
#endif