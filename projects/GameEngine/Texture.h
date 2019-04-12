#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include "GL\glew.h"
#include "GL\freeglut.h"
class Texture
{
public:
	GLuint TextureId;
	GLuint textures[15];

	Texture();
	~Texture();
	void LoadTexture(std::string filenameString, GLenum minificationFilter = GL_LINEAR, GLenum magnificationFilter = GL_LINEAR);
};
#endif;