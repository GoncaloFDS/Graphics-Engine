#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <string>
#include <fstream>
#include <map>
class Shader
{
public:
	GLuint ProgramId = 0;
	std::map<std::string, GLint> Uniforms;
	std::map<GLenum, GLuint> Shaders;
	std::map<std::string, GLint> UniformBlocks;
	///Creates a Shader Object loading a mainShader file;
	void LoadShader(GLenum ShaderType, std::string filename);
	Shader();
	~Shader();
	void Prepare();
	void Create(); // fast constructor for this specific problem
	void BindAttribute(GLuint index, const GLchar* name);
	void Link();

	void DeleteShader(GLenum ShaderType);
	void CleanCompilation();
	void Delete();
	void Detach(GLenum ShaderType);
	void checkOpenGLError(std::string error);
	void Attach(GLenum ShaderType);
	void AddUniform(const char *);
	void AddUniformBlock(const char * UniformBlockName);
	void BindUniformBlock(GLint UboID, const GLuint UboBP);
	void Enable();
	void Disable();
	GLint GetUniform(const char * UniformName);
	GLint getUniform(const char * UniformName);
	GLint GetUniformBlock(const char* UniformBlockName);
	const std::string LoadFromFile(std::string filename);
private:
	GLuint FragmentShaderId = 0, VertexShaderId = 0;
	bool isOpenGLError();
	
};

#endif