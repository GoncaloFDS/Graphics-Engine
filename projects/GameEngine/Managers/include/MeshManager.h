#pragma once
#ifndef MESHMANAGER_H
#define MESHMANAGER_H
#include <string>
#include <map>
#include "Mesh.h"
#include "IManager.h"
namespace Managers{
	class MeshManager : public IManager<MeshManager>
	{
		friend IManager<MeshManager>;
		std::map<std::string, Mesh*>* meshMap;
		~MeshManager();
		MeshManager();
	
	public:
		Mesh * create(std::string filepath, std::string meshname);
		void remove(std::string meshname);
		Mesh * get(std::string meshname);
		void add(std::string meshname, Mesh* mesh);
	};
}
#endif