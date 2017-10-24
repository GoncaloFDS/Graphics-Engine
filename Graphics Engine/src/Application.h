#pragma once
#include "Vec2.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>
#include <string>
#include "Shader.h"
#include "Vec4.h"

typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

class Application {
public:
	Application(int argc, char* argv[], Vec2 win);

private:
	static int windowHandle;
	static int frameCount;
	static Vec2 win_;
	static std::string caption;
	static Shader* shader;
	static GLuint VAO[3], VBO[3], UBO;

	static bool isOpenGlError();
	static void checkOpenGlError(std::string error);
	static void checkOpenGlInfo();

	static void timer(int value);
	static void reshape(int x, int y);
	static void idle();
	static void drawScene();
	static void display();
	static void cleanUp();

	void setUpGlut(int argc, char** argv, const Vec2 win);
	void setUpGlew();
	void setUpOpenGl() const;
	void createShaderProgram() const;
	void createCubeBuffers() const;
	void createTriangleBuffers();
	void createParallelogramBuffers();
	void createBufferObjects();
	void setUpCallBacks();

};
