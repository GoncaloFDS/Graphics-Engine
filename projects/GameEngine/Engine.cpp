#include "Engine.h"



Engine::Engine()
{
}


Engine::~Engine()
{
}

void Engine::display(float deltaTime)
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void Engine::displayWrapper()
{
}

void Engine::takeScreenShot(int winX, int winY, float currentTime) {
	BYTE* pixels = new BYTE[3 * winX * winY];
	glReadPixels(0, 0, winX, winY, GL_BGR, GL_UNSIGNED_BYTE, pixels);
	FIBITMAP* screenshot = FreeImage_ConvertFromRawBits(pixels, winX, winY, 3 * winX, 24, 0x0000FF, 0x00FF00, 0xFF0000,
		false);
	std::ostringstream os;
	os << "Screenshots/Screenshot_" << currentTime << ".bmp";
	std::string path = os.str();
	FreeImage_Save(FIF_BMP, screenshot, path.c_str(), 0);
	os.clear();
	FreeImage_Unload(screenshot);
	delete[] pixels;
}
