#include "Shader.h"
#include <iostream>
#include <vector>

void Shader::LoadShader(GLenum ShaderType, std::string filename)
{
	try {
		std::string Source = LoadFromFile(filename);
		const char *ShaderSRC = Source.c_str();
		GLuint ShaderId;
		ShaderId = glCreateShader(ShaderType);
		if (ShaderType == GL_VERTEX_SHADER) {
			VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
			ShaderId = VertexShaderId;
			glShaderSource(VertexShaderId, 1, &ShaderSRC, 0);
			glCompileShader(VertexShaderId);
		}
		else if (ShaderType == GL_FRAGMENT_SHADER) {
			FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
			ShaderId = FragmentShaderId;
			glShaderSource(FragmentShaderId, 1, &ShaderSRC, 0);
			glCompileShader(FragmentShaderId);
		}
	
		GLint result = GL_FALSE;
		int logLength;
		glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &logLength);
			char* log = new char[logLength];
			glGetShaderInfoLog(ShaderId, logLength, NULL, log);
			std::cout << std::string(log) << std::endl;
			delete log;
		}
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
		//handle exception of file not found
	}
}

Shader::Shader()
{
	//Only create the map. We can't really create the mainShader since we need to set up data before finishing the creation of the program.	
	this->Uniforms = std::map<std::string, GLint>();
	this->Shaders = std::map<GLenum, GLuint>();
	this->UniformBlocks = std::map<std::string, GLint>();
}

void Shader::Prepare() {
	this->ProgramId = glCreateProgram();
	if (this->ProgramId == 0) {
		this->checkOpenGLError("Couldn't create program.");
	}
}
Shader::~Shader()
{
}

void Shader::Create() {
	this->Prepare();
	this->Attach(GL_VERTEX_SHADER);
	this->Attach(GL_FRAGMENT_SHADER);
	this->BindAttribute(0, "in_Position");
	this->Link();
}

void Shader::BindAttribute(GLuint index, const GLchar * name)
{
	glBindAttribLocation(this->ProgramId, index, name);
	//verify error
	this->checkOpenGLError("Error Binding Atribute");
}

void Shader::Link()
{
	glLinkProgram(this->ProgramId);
	//check Error
	GLint result, logLength;
	char* log;
	glGetProgramiv(this->ProgramId, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		glGetProgramiv(this->ProgramId, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> v(logLength);
		glGetShaderInfoLog(this->ProgramId, logLength, NULL, v.data());
		std::string s(begin(v), end(v));
		std::cout << s << std::endl;
		//delete log;
	}
	this->CleanCompilation();
}

const std::string Shader::LoadFromFile(std::string filename)
{
	std::ifstream shaderFile;
	std::string Source;
	shaderFile.open(filename);

	if (shaderFile.is_open() == false) {
		throw std::exception("Couldn't open mainShader file.");
	}

	std::string lineContent = "";
	while (!shaderFile.eof()) {
		std::getline(shaderFile, lineContent);
		Source.append(lineContent + "\n");
	}
	shaderFile.close();

	return Source;
}

void Shader::DeleteShader(GLenum ShaderType) {
	switch (ShaderType) {
		case GL_VERTEX_SHADER: glDeleteShader(this->VertexShaderId); break;
		case GL_FRAGMENT_SHADER: glDeleteShader(this->FragmentShaderId); break;
	}
	this->checkOpenGLError("Error Deleting Shader : " + ShaderType);
}

void Shader::CleanCompilation() { //I can detach and flag for deletion a mainShader that has been already sucessfully linked.
	this->Detach(GL_VERTEX_SHADER);
	this->Detach(GL_FRAGMENT_SHADER);
	this->DeleteShader(GL_VERTEX_SHADER);
	this->DeleteShader(GL_FRAGMENT_SHADER);
}

void Shader::Delete()
{
	this->Disable();//Since shaders have already been detached and flagged for deletion after linking, I can just delete the program.
	glDeleteProgram(this->ProgramId);
	this->checkOpenGLError("Error deleting program");
}

void Shader::Attach(GLenum ShaderType)
{
	switch (ShaderType) {
	case GL_VERTEX_SHADER: glAttachShader(this->ProgramId, this->VertexShaderId); break;
	case GL_FRAGMENT_SHADER: glAttachShader(this->ProgramId, this->FragmentShaderId); break;
	}

	this->checkOpenGLError("Error Attaching Shader : " + ShaderType);

}

void Shader::AddUniform(const char *UniformName)
{
	GLint UniformLocation = glGetUniformLocation(this->ProgramId, UniformName);
	//check errors
	if (UniformLocation == -1) {
		//uniform is not active
		this->checkOpenGLError("Uniform doesn't seem to be active in any mainShader.");
		//handle error
	}
	else {
		Uniforms.insert(std::pair<std::string, GLint>(std::string(UniformName), UniformLocation));
	}
}

void Shader::AddUniformBlock(const char * UniformBlockName)
{
	GLint Ubo = glGetUniformBlockIndex(this->ProgramId, UniformBlockName);
	this->checkOpenGLError("Error getting Uniform Block Index");
	this->UniformBlocks.insert(std::pair<std::string, GLint>(std::string(UniformBlockName), Ubo));
}

void Shader::BindUniformBlock(GLint UboID, const GLuint UboBP)
{
	glUniformBlockBinding(this->ProgramId, UboID, UboBP);
	this->checkOpenGLError("Error Binding Uniform Block");
}

void Shader::Enable()
{
	glUseProgram(this->ProgramId);
	this->checkOpenGLError("Error enabling Program : " + this->ProgramId);
}

void Shader::Disable()
{
	glUseProgram(0);
	this->checkOpenGLError("Error disabling Program " + this->ProgramId);
}

GLint Shader::GetUniform(const char *UniformName) {
	auto Uniform = Uniforms.find(std::string(UniformName));
	if (Uniform != Uniforms.end()) {
		return Uniform->second;
	}
	else {
		return -1;
	}
}

GLint Shader::getUniform(const char * UniformName)
{
	return glGetUniformLocation(this->ProgramId, UniformName);
}

GLint Shader::GetUniformBlock(const char * UniformBlockName)
{
	auto UniformBlock = UniformBlocks.find(std::string(UniformBlockName));
	if (UniformBlock != UniformBlocks.end()) {
		return UniformBlock->second;
	}
	else {
		return 0;
	}
}

void Shader::Detach(GLenum ShaderType)
{
	switch (ShaderType) {
	case GL_VERTEX_SHADER: glDetachShader(this->ProgramId, this->VertexShaderId); this->VertexShaderId = 0; break;
	case GL_FRAGMENT_SHADER: glDetachShader(this->ProgramId, this->FragmentShaderId); this->FragmentShaderId = 0; break;
	}
	checkOpenGLError("Error detaching mainShader :" + ShaderType);
}

bool Shader::isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void Shader::checkOpenGLError(std::string error)
{
	if (this->isOpenGLError()) {
		std::cerr << error << std::endl;
		std::cin.get();
		exit(EXIT_FAILURE);
	}
}
