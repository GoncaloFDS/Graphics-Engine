#pragma once
#include "Vec2.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>
#include <string>
#include "Shader.h"
#include "Vec4.h"
#include "Camera.h"

typedef struct {
	GLfloat XYZW[4];
} Vertex;

enum Projection {
	Ortho, Perspective
};

class Application {
public:
	Application(int argc, char* argv[], Vec2 win);

private:
	static int windowHandle;
	static int frameCount;
	static Vec2 win_;
	static std::string caption;
	static Shader* shader;
	static Projection projectionType;
	static GLuint VAO[3], VBO[3];
	static Camera camera;
	static int oldTime;
	static float deltaTime;
	static bool movementKeyPressed[6];
	
	static void createSceneMatrices();

	static bool isOpenGlError();
	static void checkOpenGlError(std::string error);
	static void checkOpenGlInfo();

	static void timer(int value);
	static void reshape(int x, int y);
	static void idle();
	static void drawScene();
	static void display();
	static void cleanUp();
	static void keyboardInput(unsigned char key, int x, int y);
	static void mouseMoveInput(int x, int y);
	static void mouseButtonInput(int button, int state, int x, int y);

	void setUpGlut(int argc, char** argv, const Vec2 win);
	void setUpGlew();
	void setUpOpenGl() const;
	void createShaderProgram() const;
	static void switchProjection();
	static void keyUpFunc(unsigned char key, int x, int y);
	static void processMovement();
	void setUpCallBacks();

};
