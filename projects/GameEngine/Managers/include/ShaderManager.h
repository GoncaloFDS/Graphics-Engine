#pragma once
#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <map>
#include <iostream>
#include "Shader.h"
#include "IManager.h"
namespace Managers{

	class ShaderManager : public IManager<ShaderManager> {
	
		friend IManager<ShaderManager>;
		ShaderManager();
		~ShaderManager();
		std::map <std::string, Shader*>* _shaders;
	public:
		Shader * create(std::string shadername, std::string vertshader_path, std::string fragshader_path);
		void remove(std::string shadername);
		Shader * get(std::string shadername);
		void add(std::string shadername, Shader* shader);
		std::map<std::string, Shader*> getShaders();
	};
}

#endif