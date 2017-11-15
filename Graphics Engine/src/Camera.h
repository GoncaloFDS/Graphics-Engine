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
	float MovementSpeed;
	float MouseSensivity;

	Vec2 LastMouse;
	bool LeftButtonPressed = false;
	bool FirstMouseInput = true;
	Vec3 RotationRodrigues;
	float ZoomDistance;
	Quat Q;
	Mat4 R;
	Vec3 RotationDeltaQ;

public:
	Camera() = default;
	Camera(Vec3 pos, Vec3 center);
	~Camera() = default;
	Vec3 Position;
	Mat4 ViewMatrix;
	bool UsingQuaternions;

	void setLeftButton(bool b);
	void setIsFirstMouseInput(bool b);

	void moveCamera(movementDir dir, float deltaTime);
	void moveMouse(int x, int y, Vec2 screen);
	Mat4 getViewMatrix();

};

