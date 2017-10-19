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
	// Constructor generates the shader on the fly
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;
		// ensures ifstream objects can throw exceptions:
		vertexShaderFile.exceptions(std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::badbit);
		try	{
			// Open files
			vertexShaderFile.open(vertexPath);
			fragmentShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vertexShaderFile.rdbuf();
			fShaderStream << fragmentShaderFile.rdbuf();
			// close file handlers
			vertexShaderFile.close();
			fragmentShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar *vertexShaderCode = vertexCode.c_str();
		const GLchar *fragmentShaderCode = fragmentCode.c_str();
		
		GLint success;
		GLchar infoLog[512];
		// Vertex Shader
		GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexId, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexId);
		// Print compile errors if any
		glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Fragment Shader
		const GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentId, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentId);
		// Print compile errors if any
		glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Shader programId
		programId = glCreateProgram();
		glAttachShader(programId, vertexId);
		glAttachShader(programId, fragmentId);
		glLinkProgram(programId);
		// Print linking errors if any
		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->programId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);

	}
	// Uses the current shader
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
