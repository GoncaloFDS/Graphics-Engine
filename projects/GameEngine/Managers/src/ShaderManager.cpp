#include "ShaderManager.h"

using namespace Managers;

ShaderManager::ShaderManager()
{
	if (this->_shaders == nullptr)
		this->_shaders = new std::map<std::string, Shader*>();
}


ShaderManager::~ShaderManager()
{
	for (auto it = this->_shaders->begin(); it != this->_shaders->end(); ++it) {
		delete(it->second);
	}
	_shaders->clear();
	delete _shaders;
}

Shader * Managers::ShaderManager::create(std::string shadername, std::string vertshader_path, std::string fragshader_path)
{
	try {
		Shader* shr = new Shader();
		shr->LoadShader(GL_VERTEX_SHADER, vertshader_path);
		shr->LoadShader(GL_FRAGMENT_SHADER, fragshader_path);
		shr->Prepare();
		shr->Attach(GL_VERTEX_SHADER);
		shr->Attach(GL_FRAGMENT_SHADER);
		shr->BindAttribute(0, "inPosition");
		shr->BindAttribute(1, "inTexCoord");
		shr->BindAttribute(2, "inNormal");
		shr->BindAttribute(3, "inTangent");
		shr->BindAttribute(4, "inBiTangent");
		shr->Link();	
		this->add(shadername, shr);
		return this->get(shadername);
	}
	catch (std::ios_base::failure e) {
		std::cout << "Texture not found: " << e.what() << std::endl;
		return nullptr;
	}
}

void Managers::ShaderManager::remove(std::string shadername)
{
	this->_shaders->erase(shadername);
}

Shader * Managers::ShaderManager::get(std::string shadername)
{
	auto found = this->_shaders->find(shadername);
	if (found != this->_shaders->end()) {
		return found->second;
	}
	return nullptr;
}

void Managers::ShaderManager::add(std::string shadername, Shader * shr)
{
	(*_shaders)[shadername] = shr;
}


std::map<std::string, Shader*> Managers::ShaderManager::getShaders() {
	return *this->_shaders;
}