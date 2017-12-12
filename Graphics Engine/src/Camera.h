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
	Mat4 projMat;
	Mat4 cubeMat;
	
public:
	Camera() = default;
	Camera(Vec3 pos, Vec3 center);
	~Camera() = default;
	Mat4 getProjectionMatrix();
	void setProjectionMatrix(const Mat4& mat4);
	void invertPitch();
	Vec3 position;
	Mat4 viewMatrix;
	bool UsingQuaternions;

	void setLeftButton(bool b);
	void setIsFirstMouseInput(bool b);

	void moveCamera(movementDir dir, float deltaTime);
	void moveMouse(int x, int y, Vec2 screen);
	Mat4 getCubeMatrix();
	Mat4 getViewMatrix();

};

