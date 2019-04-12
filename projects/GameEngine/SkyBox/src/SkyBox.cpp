#include "SkyBox\include\SkyBox.h"

SkyBox::SkyBox(std::vector<std::string> faces)
{
	this->skybox = new CubeMap(faces);
	Mesh* cube = MeshManager::getInstance()->get("skyboxcube");
	if (cube == nullptr) {
		this->cube = MeshManager::getInstance()->create("Assets/Models/cubemap_cube.obj", "skyboxcube");
	}
	else {
		this->cube = cube;
	}
}

SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
	delete skybox;
}

void SkyBox::setMesh(Mesh * cube)
{
	this->cube = cube;
}

void SkyBox::setCubeMap(CubeMap * map)
{
	this->skybox = map;
}

void SkyBox::setSkyBoxShader(Shader * shader)
{
	this->skyboxShader = shader;
}

