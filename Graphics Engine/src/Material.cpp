#include "Material.h"
#include <string>
#include <fstream>
#include <sstream>

Material::Material()
{
}


Material::~Material()
{
}

void Material::LoadMaterial(std::string filepath)
{
	std::ifstream ifile(filepath);
	std::string line;
	while (std::getline(ifile, line))
		parseLine(std::stringstream(line));

	std::cout << "Shininess: " << this->shininess << std::endl;

	std::cout << "Ka: " << this->ambient << std::endl;

	std::cout << "Kd: " << this->diffuse << std::endl;

	std::cout << "Ke: " << this->emissive << std::endl;

	std::cout << "Ks: " << this->specular << std::endl;
}

void Material::parseLine(std::stringstream& sin)
{
	std::string s;
	sin >> s;
	if (s.compare("Ns") == 0) sin >> this->shininess;
	else if (s.compare("Ka") == 0) sin >> this->ambient;
	else if (s.compare("Kd") == 0) sin >> this->diffuse;
	else if (s.compare("Ke") == 0) sin >> this->emissive;
	else if (s.compare("Ks") == 0) sin >> this->specular;
}


