#include "Water.h"
#include "Vector2.h"

void Water::setShaderUniforms(Shader* shader) {
	shader->Enable();

	glUniform1i(shader->getUniform("ReflectionTexture"), 0);
	glUniform1i(shader->getUniform("RefractionTexture"), 1);
	glUniform1i(shader->getUniform("depthMap"), 4);
	glUniform1f(shader->getUniform("near"), 0.1); // near and far from the camera
	glUniform1f(shader->getUniform("far"), 50);
	glUniform1f(shader->getUniform("waveStrenght"), .01);
	glUniform1f(shader->getUniform("reflectiveness"), 2);
	glUniform1f(shader->getUniform("shineDamper"), 30.0);
	glUniform1f(shader->getUniform("waterClampValue"), 0.1);
	Vector3 lightColor(0.96, 0.96, 0.74);
	glUniform3fv(shader->getUniform("lightColor"), 1, lightColor.coordinates);

	shader->Disable();
}

Water::Water(int X, int Y) {
	setDimensions(X, Y);
	initialiseReflectionFrameBuffer();
	initialiseRefractionFrameBuffer();
}

void Water::bindReflectionBuffer() {
	bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void Water::bindRefractionBuffer() {
	bindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

GLuint Water::createFrameBuffer() {
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	return frameBuffer;
}

void Water::bindFrameBuffer(GLuint frameBuffer, const int width, const int height) {

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}

void Water::unbindCurrentFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Vector2 vp = Vector2(this->Width, this->Height);
	glViewport(0, 0, vp.coordinates[0], vp.coordinates[1]);
}


GLuint Water::getReflectionTexture() {
	return reflectionTexture;
}

GLuint Water::getRefractionTexture() {
	return refractionTexture;
}

GLuint Water::getRefractionDepthTexture() {
	return refractionDepthTexture;
}

Water::~Water() {
	cleanUp();
}

void Water::initialiseReflectionFrameBuffer() {
	reflectionFrameBuffer = createFrameBuffer();
	reflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	unbindCurrentFrameBuffer();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "initialiseReflectionFrameBuffer error" << std::endl;
}

void Water::initialiseRefractionFrameBuffer() {
	refractionFrameBuffer = createFrameBuffer();
	refractionTexture = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	unbindCurrentFrameBuffer();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "initialiseRefractionFrameBuffer error" << std::endl;
}

void Water::setDimensions(int w, int h) {
	this->Width = w;
	this->Height = h;
}

GLuint Water::createTextureAttachment(const int width, const int height) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
	             0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "createTextureAttachment error" << std::endl;
	return texture;
}

GLuint Water::createDepthTextureAttachment(const int width, const int height) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
	             0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "createDepthTextureAttachment error" << std::endl;
	return texture;
}

GLuint Water::createDepthBufferAttachment(const int width, const int height) {
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "createDepthBufferAttachment error" << std::endl;
	return depthBuffer;
}

void Water::calculateMoveFactor(const GLint uniform, const float deltaTime) {

	moveFactor += WATER_SPEED * deltaTime;
	if (moveFactor >= 1) moveFactor = 0.f;
	glUniform1f(uniform, moveFactor);
}

void Water::bindTextures() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, getReflectionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, getRefractionTexture());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, getRefractionDepthTexture());
}


void Water::cleanUp() {
	glDeleteFramebuffers(1, &reflectionFrameBuffer);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteRenderbuffers(1, &reflectionDepthBuffer);
	glDeleteFramebuffers(1, &refractionFrameBuffer);
	glDeleteTextures(1, &refractionTexture);
	glDeleteTextures(1, &refractionDepthTexture);
	glDisable(GL_BLEND);
}
