#pragma once
#ifndef MOUSE_H
#define MOUSE_H
#include "Vector2.h"
#include "GL\freeglut.h"

class Mouse
{
public:
	Vector2 Position;
	Vector2 LastPosition;
	float MouseX = 0;
	float MouseY = 0;
	float oldAngleX = 0;
	float oldAngleY = 0;
	bool FirstClick = true;
	float Sensivity = 0.002f;
	bool MouseState = false;
	void Movement(int x, int y);
	Mouse();
	
	~Mouse();
};

#endif