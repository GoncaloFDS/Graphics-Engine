#include "TextureManager.h"
using namespace Managers;


TextureManager::TextureManager()
{
	if (this->_textures == nullptr)
		this->_textures = new std::map<std::string, Texture*>();
}


TextureManager::~TextureManager()
{
	for (auto it = this->_textures->begin(); it != this->_textures->end(); ++it) {
		delete(it->second);
	}
	_textures->clear();
	delete _textures;
}

Texture * Managers::TextureManager::create(std::string filepath, std::string texturename)
{
	try {
		Texture* tex = new Texture();
		tex->LoadTexture(filepath);
		this->add(texturename, tex);
		return this->get(texturename);
	}
	catch (std::ios_base::failure e) {
		std::cout << "Texture not found: " << e.what() << std::endl;
		return nullptr;
	}
}

void Managers::TextureManager::remove(std::string texturename)
{
	this->_textures->erase(texturename);
}

Texture * Managers::TextureManager::get(std::string texturename)
{
	auto found = this->_textures->find(texturename);
	if (found != this->_textures->end()) {
		return found->second;
	}
	return nullptr;
}

void Managers::TextureManager::add(std::string texturename, Texture * texture)
{
	(*_textures)[texturename] = texture;
}
