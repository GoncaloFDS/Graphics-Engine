#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	const GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, Vertex);

	const GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, Fragment);

	Id = glCreateProgram();
	glAttachShader(Id, vertex);
	glAttachShader(Id, fragment);
	glLinkProgram(Id);
	checkCompileErrors(Id, Program);
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader() {
	glDeleteProgram(Id);
}

void Shader::use() const {
	glUseProgram(Id);
}

void Shader::detach() {
	glUseProgram(0);
}

GLint Shader::getUniform(const char* uniformName) const {
	return glGetUniformLocation(Id, uniformName);
}

GLuint Shader::bindAttribute(const char* attributeName) {
	// Add the attribute location value for the attributeName key
	AttributeMap[attributeName] = glGetAttribLocation(Id, attributeName);

	// Check to ensure that the ShaderProgram contains an attribute with this name
	if (AttributeMap[attributeName] == -1) {
		std::cout << "Could not add attribute: " << attributeName;
	}
	// Return the attribute location
	return AttributeMap[attributeName];
}


void Shader::checkCompileErrors(const GLuint shader, const Type type) const {
	GLint success;
	char infoLog[1024];
	if (type != Program) {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog <<
				"\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog <<
				"\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
