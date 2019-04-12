#pragma once
#include "QuaternionCamera.h"
#include "AnimationManager.h"
#include "MeshManager.h"
#include "SceneManager.h"
#include "Mouse.h"
#include "Keyboard.h"
#define BACKGROUND_COLOR Vector4(0.8f, 0.8f, 0.8f, 1.0f);

class Engine
{
	Engine* app;
	int FPS = 60;
	int FrameCount = 0;
	int winX = 640;
	int winY = 480;

public:
	Engine();
	~Engine();

	float frameTimeElapsed;
	static void takeScreenShot(int winX, int winY, float currentTime);


	protected:
		QuaternionCamera* camera;
		Mouse* mouse;
		virtual void init(int arc, char* argv[]);
		virtual void display(float deltaTime);
		virtual void reshape();
		virtual void TakeScreenshot();
		virtual void init();
		virtual void drawScene();
		virtual void idle();
		virtual void timer(int value);
		virtual void handleKeyUp(unsigned char Key, int x, int y);
		virtual void createAnimation();
		virtual void mouseHandle(int button, int state, int x, int y);
		virtual void handleKeyDown(unsigned char Key, int x, int y);
		virtual void mouseWheel(int button, int dir, int x, int y);
		virtual void setupCallBacks();
		virtual void setupOpenGL();
		virtual void setupGLEW();
		virtual void setupGLUT(int argc, char* argv[]);
		
		static void displayWrapper();
		static void reshapeWrapper(int width, int height);
		static void runWrapper();
		static void mouseButtonPressWrapper(int button, int state, int x, int y);
		static void mouseMoveWrapper(int x, int y);
		static void keyboardDownWrapper(unsigned char key, int x, int y);
		static void keyboardUpWrapper(unsigned char key, int x, int y);
		static void specialKeyboardDownWrapper(int key, int x, int y);
		static void specialKeyboardUpWrapper(int key, int x, int y);

};

