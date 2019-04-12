#include "MeshManager.h"

using namespace Managers;

Managers::MeshManager::~MeshManager()
{
	for (auto it = this->meshMap->begin(); it != this->meshMap->end(); ++it) {
		delete(it->second);
	}
	meshMap->clear();
	delete meshMap;
}

MeshManager::MeshManager() {
	if(this->meshMap == nullptr)
		this->meshMap = new std::map<std::string, Mesh*>();
}
Mesh * MeshManager::create(std::string filepath, std::string meshname)
{
	try{
	Mesh* mesh = new Mesh(filepath);
	this->add(meshname, mesh);
	return this->get(meshname);
	}
	catch (std::ios_base::failure e) {
		std::cout << "Mesh not found: " << e.what() << std::endl;
		return nullptr;
	}
}

void MeshManager::remove(std::string meshname)
{
	this->meshMap->erase(meshname);
}

Mesh * MeshManager::get(std::string meshname)
{
	auto found = this->meshMap->find(meshname);
	if (found != this->meshMap->end()) {
		return found->second;
	}
	return nullptr;
	//return this->meshMap->find(meshname)->second;
}

void MeshManager::add(std::string meshname, Mesh* mesh)
{
	(*meshMap)[meshname] = mesh;
}

/*
MeshManager::~MeshManager()
{
	delete(instance);
}
*/


