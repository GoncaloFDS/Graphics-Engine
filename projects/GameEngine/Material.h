#pragma once
#include "Vector3.h"

class Material
{

public:
	Material();
	~Material();
	void LoadMaterial(std::string filepath);
	void parseLine(std::stringstream& sin);
	float shininess;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	Vector3 emissive;
};

