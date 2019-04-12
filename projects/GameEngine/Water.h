#pragma once

#include <GL/glew.h>
#include "Vector3.h"
#include "Shader.h"

class Water {
public:
	int REFLECTION_WIDTH = 1280;
	int REFLECTION_HEIGHT = 720;

	int REFRACTION_WIDTH = 1280;
	int REFRACTION_HEIGHT = 720;
	
	const float WATER_SPEED = 0.02f;
	float moveFactor = 0.f;
	
	Water (int x, int y);
	void bindReflectionBuffer();
	void bindRefractionBuffer();
	void unbindCurrentFrameBuffer();
	GLuint getReflectionTexture();
	GLuint getRefractionTexture();
	GLuint getRefractionDepthTexture();
	void calculateMoveFactor(GLint uniform, const float deltaTime);
	void setShaderUniforms(Shader* shader);
	void bindTextures();
	~Water();

	void setDimensions(int w, int h);
private:
	int Width;
	int Height;
	GLuint reflectionFrameBuffer;
	GLuint reflectionTexture;
	GLuint reflectionDepthBuffer;

	GLuint refractionFrameBuffer;
	GLuint refractionTexture;
	GLuint refractionDepthTexture;

	void initialiseReflectionFrameBuffer();
	void initialiseRefractionFrameBuffer();

	GLuint createFrameBuffer();
	void bindFrameBuffer(GLuint frameBuffer, const int width, const int height);
	
	GLuint createTextureAttachment(int width, int height);
	GLuint createDepthTextureAttachment(const int width, const int height);
	GLuint createDepthBufferAttachment(const int width, const int height);
	void cleanUp();

};

