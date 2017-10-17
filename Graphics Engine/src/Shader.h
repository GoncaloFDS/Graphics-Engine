#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader {
private:
	GLuint vertexIndex = 0;
	GLuint ColorIndex = 1;

public:
	GLuint programId;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;
		vertexShaderFile.exceptions(std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::badbit);
		try	{
			vertexShaderFile.open(vertexPath);
			fragmentShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vertexShaderFile.rdbuf();
			fShaderStream << fragmentShaderFile.rdbuf();

			vertexShaderFile.close();
			fragmentShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR: Couldnt read shader file" << std::endl;
		}
		const GLchar *vertexShaderCode = vertexCode.c_str();
		const GLchar *fragmentShaderCode = fragmentCode.c_str();
		
		GLint success;
		GLchar infoLog[512];
		// Vertex Shader
		GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexId, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexId);

		glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexId, 512, NULL, infoLog);
			std::cout << "ERROR: Vertex Shader failed to compile\n" << infoLog << std::endl;
		}
		// Fragment Shader
		const GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentId, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentId);

		glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentId, 512, NULL, infoLog);
			std::cout << "ERROR: Fragment Shader failed to compile\n" << infoLog << std::endl;
		}

		programId = glCreateProgram();
		glAttachShader(programId, vertexId);
		glAttachShader(programId, fragmentId);
		glLinkProgram(programId);

		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->programId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);

	}
	void Use() {
		glUseProgram(programId);
	}

	void addAttribute(const char* attributeName) {
		glBindAttribLocation(programId, vertexIndex, attributeName);
	}

	GLint getUniform(const char* uniformName) {
		return glGetUniformLocation(programId, uniformName);
	}
};
