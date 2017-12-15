#pragma once
#include "Vec3.h"

class Material
{

public:
	Material();
	~Material();
	void LoadMaterial(std::string filepath);
	void parseLine(std::stringstream& sin);
	float shininess;
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	Vec3 emissive;
};

