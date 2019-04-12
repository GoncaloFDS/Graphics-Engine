#pragma once
#ifndef CUBEMAP_H
#define CUBEMAP_H
#include <string>
#include <vector>
#include "GL\glew.h"
#include "FreeImage.h"
#include <iostream>
#define CUBEMAP_FOLDER "Assets/Textures/cubemap/"
class CubeMap
{
public:
	CubeMap();
	CubeMap(std::vector<std::string> faces);
	~CubeMap();
	unsigned int textureId;
	void LoadCubeMap(std::vector<std::string> faces);
};

#endif