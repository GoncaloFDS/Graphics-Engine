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
	Vec3 worldUp;

	float movementSpeed;
	float mouseSensivity;

	float yaw;
	float pitch;

public:
	Camera() = default;
	Camera(Vec3 pos);
	~Camera() = default;
	Vec3 position;

	void moveCamera(movementDir dir, float deltaTime);
	void updateVectors();
	void moveMouse(int xOffset, int yOffset);
	Mat4 getViewMatrix();

};

