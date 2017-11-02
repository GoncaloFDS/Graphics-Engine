#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include "Mat4.h"

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
	Vec3 front;
	Vec3 right;
	Vec3 up;
	Vec3 worldUp;

	float movementSpeed;
	float mouseSensivity;

	float yaw;
	float pitch;
	Vec2 lastMouse;
	bool leftButtonPressed = false;
	bool firstMouseInput = true;
	float frontDist;

public:
	Camera() = default;
	Camera(Vec3 pos, Vec3 center);
	~Camera() = default;
	Vec3 position;
	Mat4 viewMatrix;

	void setLeftButton(bool b);
	void setIsFirstMouseInput(bool b);

	void moveCamera(movementDir dir, float deltaTime);
	void updateVectors();
	void moveMouse(int x, int y, Vec2 screen);
	Mat4 getViewMatrix() const;

};

