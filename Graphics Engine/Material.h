#pragma once
#include "Vec3.h"

class Material
{
	float shininess;
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	Vec3 emissive;

public:
	Material();
	~Material();
};

