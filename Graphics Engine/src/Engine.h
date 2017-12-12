#pragma once
#include "Vec2.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>
#include <string>
#include "Shader.h"
#include "Vec4.h"
#include "Camera.h"
#include "SceneNode.h"
#include "MatrixFactory.h"
#include "Shader.h"
#include "Mesh.h"
#include "SceneGraph.h"
#include "Animation.h"
#include "Animator.h"
#include "Water.h"
#include "CubeMap.h"


typedef struct {
	GLfloat XYZW[4];
} Vertex;

enum Projection {
	Ortho, Perspective
};

class Engine {
public:
	Engine(int argc, char* argv[], Vec2 win);
	static Vec2 getDisplaySize();
	static void checkOpenGlError(std::string error);

private:
	static int WindowHandle;
	static int FrameCount;
	static Vec2 WindowSize;
	static std::string Caption;
	static Shader* simpleShader;
	static Projection ProjectionType;
	static Camera MainCamera;
	static int OldTime;
	static float DeltaTime;
	static bool MovementKeyPressed[8];
	
	static void createSceneMatrices();
	static void createAnimations(std::vector<SceneNode*> nodes);
	static void createScene();

	static bool isOpenGlError();
	
	static void checkOpenGlInfo();

	static void timer(int value);
	static void reshape(int x, int y);
	
	static void idle();
	static void updateViewProjectionMatrices();
	static void drawScene();
	static void display();
	static void cleanUp();
	static void keyboardInput(unsigned char key, int x, int y);
	static void mouseMoveInput(int x, int y);
	static void mouseButtonInput(int button, int state, int x, int y);

	void setUpGlut(int argc, char** argv, const Vec2 win);
	void setUpGlew();
	void setUpOpenGl() const;
	void createShaders() const;
	static void switchProjection();
	static void keyUpFunc(unsigned char key, int x, int y);
	static void processMovement();
	void setUpCallBacks();

};
