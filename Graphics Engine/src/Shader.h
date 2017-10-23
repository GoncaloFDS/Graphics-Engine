#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader {
public:
	// the program ID
	GLuint ID;

	enum Type {
		Program,
		Vertex,
		Fragment
	};

	// constructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();

	void use() const;
	GLint getUniform(const char* str) const;

private:
	void checkCompileErrors(const GLuint shader, const Type type) const;
};

#endif
