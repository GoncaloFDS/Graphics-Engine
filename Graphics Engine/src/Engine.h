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

class Engine {
public:
	Engine(int argc, char* argv[], Vec2 win);

private:
	static int WindowHandle;
	static int FrameCount;
	static Vec2 WindowSize;
	static std::string Caption;
	static Shader* ShaderProgram;
	static Projection ProjectionType;
	static Camera MainCamera;
	static int OldTime;
	static float DeltaTime;
	static bool MovementKeyPressed[8];
	
	static void createSceneMatrices();
	static void createTangram();

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
