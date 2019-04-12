#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include "Mesh.h"
#include "MeshManager.h"
#include "CubeMap.h"
using namespace Managers;
class SkyBox {

public:
	Mesh* cube;
	CubeMap* skybox;
	Shader* skyboxShader;
	SkyBox(std::vector<std::string> faces);
	SkyBox();
	~SkyBox();
	void setMesh(Mesh* cube);
	void setCubeMap(CubeMap* map);
	void setSkyBoxShader(Shader* shader);
};




#endif;