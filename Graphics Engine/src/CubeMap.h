#pragma once

#include <GL/glew.h>
#include <vector>
#include "Vec3.h"
#include "Vec2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Shader.h"
#include "Engine.h"


class CubeMap {
	

	GLuint vbo;
	GLuint vao;
	GLuint texCube;
	Shader* shader;

	bool loadCubeMapSide(GLuint texture, GLenum sideTarget, const char* fileName);

public:
	CubeMap();
	void createCubeMap(const char* front, const char* back, const char* top,
		const char* bottom, const char* left, const char* right);
	void draw();
	void setShader(Shader* s);
};