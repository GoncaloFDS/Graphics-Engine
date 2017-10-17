#pragma once
#include "CGMath/Vec2.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <string>


class Application {

public:
	Application(Vec2 win, const char* caption);
	GLFWwindow* getWindow();

private:
	GLFWwindow* m_window;
	
	void setupGLFW(Vec2 win, const char* caption);
	void setupGLEW();
	void setupOpenGL();

	void checkOpenGLInfo();
};

