#pragma once
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "IManager.h"
#include <map>
#include <iostream>
#include "Texture.h"
namespace Managers{
	class TextureManager : public IManager<TextureManager>
	{
		friend IManager<TextureManager>;
		std::map < std::string, Texture*>* _textures;
		TextureManager();
		~TextureManager();

	public:
		Texture * create(std::string filepath, std::string texturename);
		void remove(std::string texturename);
		Texture * get(std::string texturename);
		void add(std::string texturename, Texture* texture);
	};
}
#endif