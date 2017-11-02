#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include "Mat4.h"
#include "Quat.h"

#define PI 3.1415927

enum movementDir {
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down
};

class Camera {
private:
	float movementSpeed;
	float mouseSensivity;

	Vec2 lastMouse;
	bool leftButtonPressed = false;
	bool firstMouseInput = true;
	Vec3 rotationAngle;
	float zoomDistance;
	Quat q;
	Vec3 rotationDelta;

public:
	Camera() = default;
	Camera(Vec3 pos, Vec3 center);
	~Camera() = default;
	Vec3 position;
	Mat4 viewMatrix;
	bool usingQuaternions;

	void setLeftButton(bool b);
	void setIsFirstMouseInput(bool b);

	void moveCamera(movementDir dir, float deltaTime);
	void moveMouse(int x, int y, Vec2 screen);
	Mat4 getViewMatrix();

};

