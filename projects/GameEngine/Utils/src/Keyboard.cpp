#include "Keyboard.h"


Keyboard* Keyboard::instance = nullptr;

Keyboard::Keyboard()
{
}


Keyboard* Keyboard::getInstance()
{
	if (instance == nullptr) {
		instance = new Keyboard();
	}
	return instance;
}

void Keyboard::pressKey(char key)
{
	char lower = std::tolower(key);
	this->KeyStates[lower] = true;
}

bool Keyboard::isKeyPressed(char key)
{
	char lower = std::tolower(key);
	return this->KeyStates[lower];
}

void Keyboard::releaseKey(char key)
{
	char lower = std::tolower(key);
	this->KeyStates[lower] = false;
}

Keyboard::~Keyboard()
{
	delete(instance);
}
