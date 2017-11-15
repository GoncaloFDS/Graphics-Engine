#include <cstdlib>
#include <iostream>
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "Vec2.h"
#include "Engine.h"

void init(int argc, char* argv[]) {
	Engine app(argc, argv, Vec2(640, 480));

}

int main(int argc, char* argv[]) {
	try {
		init(argc, argv);
		glutMainLoop();

		exit(EXIT_SUCCESS);
	}
	catch (std::exception &e) {
		std::cerr << "Caught Exception:" << e.what() << std::endl;
		std::cin.get();
	}

}
