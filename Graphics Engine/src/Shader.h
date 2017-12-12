#pragma once

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>


class Shader {
public:
	GLuint Id;
	std::map<std::string, int> AttributeMap;

	enum Type {
		Program,
		Vertex,
		Fragment
	};

	// constructor reads and builds the simpleShader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();

	void use() const;
	void detach();
	GLint getUniform(const char* str) const;
	GLuint bindAttribute(const char* attrbName);

private:
	void checkCompileErrors(const GLuint shader, const Type type) const;
};
