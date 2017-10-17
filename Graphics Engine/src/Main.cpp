#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "CGMath/Vec2.h"
#include "Application.h"


int main(int argc, char* argv[]) {
	Application app(Vec2(800, 600), "Graphics Engine");
	GLFWwindow* window = app.getWindow();

	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}