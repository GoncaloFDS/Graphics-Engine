#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <cctype>
class Keyboard
{
	static Keyboard* instance;
	bool KeyStates[256] = { false };
	Keyboard();
	~Keyboard();
//	void PressDownKey(char *);
//	void PressUpKey(char *);
public:
	static Keyboard* getInstance();
	void pressKey(char key);
	bool isKeyPressed(char key);
	void releaseKey(char key);
};

#endif // !KEYBOARD_H