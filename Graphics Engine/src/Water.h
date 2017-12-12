#pragma once

#include <GL/glew.h>

class Water {
public:
	int REFLECTION_WIDTH = 1280;
	int REFLECTION_HEIGHT = 720;

	int REFRACTION_WIDTH = 1280;
	int REFRACTION_HEIGHT = 720;

	
	Water ();
	void bindReflectionBuffer();
	void bindRefractionBuffer();
	void unbindCurrentFrameBuffer();
	GLuint getReflectionTexture();
	GLuint getRefractionTexture();
	GLuint getRefractionDepthTexture();
	~Water();

private:

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

