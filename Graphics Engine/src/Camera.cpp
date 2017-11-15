#include "Camera.h"
#include "MatrixFactory.h"
#include <GL/glew.h>
#include "Engine.h"
#include "Quat.h"

float radians(const float degrees) {
	return degrees * PI / 180;
}

Camera::Camera(const Vec3 pos, const Vec3 center) {
	Position = pos;
	MovementSpeed = 3.f;
	MouseSensivity = 0.005f;
	RotationRodrigues = { 0, 0, 0 };
	RotationDeltaQ = { 0, 0, 0 };
	ViewMatrix = MatrixFactory::Translate(pos);
	ZoomDistance = 0;
	UsingQuaternions = false;
	Q = Quat(1, 0, 0, 0);
	R = MatrixFactory::Identity();
}


void Camera::setLeftButton(const bool b) {
	LeftButtonPressed = b;
}

void Camera::setIsFirstMouseInput(const bool b) {
	FirstMouseInput = b;
}

void Camera::moveCamera(const movementDir dir, const float deltaTime) {
	const float velocity = MovementSpeed * deltaTime;
	if (dir == Forward)
		ZoomDistance += velocity;
	if (dir == Backward)
		ZoomDistance -= velocity;
	if (dir == Up) {
		RotationRodrigues.z += velocity;
		RotationDeltaQ.z = velocity;
	}
	if (dir == Down) {
		RotationRodrigues.z -= velocity;
		RotationDeltaQ.z = -velocity;
	}
}

void Camera::moveMouse(int x, int y, Vec2 screen) {
	if (LeftButtonPressed) {
		if (FirstMouseInput) {
			LastMouse.x = x;
			LastMouse.y = y;
			FirstMouseInput = false;
		}

		RotationRodrigues.x += (LastMouse.x - x) * MouseSensivity;
		RotationRodrigues.y += (LastMouse.y - y) * MouseSensivity;

		RotationDeltaQ.x = (LastMouse.x - x) *0.5f ;
		RotationDeltaQ.y = (LastMouse.y - y) *0.5f;

		LastMouse.x = x;
		LastMouse.y = y;

	}
}

Mat4 Camera::getViewMatrix() {
	if(!UsingQuaternions) {
		R = MatrixFactory::Rotate(RotationRodrigues.x, Vec3(0, 1, 0)) *
			MatrixFactory::Rotate(RotationRodrigues.y, Vec3(1, 0, 0)) *
			MatrixFactory::Rotate(RotationRodrigues.z, Vec3(0, 0, 1)) ;

		ViewMatrix = MatrixFactory::Translate(Vec3(0, 0, Position.z + ZoomDistance)) * R;

	}
	else {
		Q = Quat(RotationDeltaQ.x, Vec4(0, 1, 0, 1)) * 
			Quat(RotationDeltaQ.y, Vec4(1, 0, 0, 1)) * 
			Quat(RotationDeltaQ.z * 50, Vec4(0, 0, 1, 1)) * Q;
		RotationDeltaQ = { 0, 0, 0 };
		ViewMatrix = MatrixFactory::Translate(Vec3(0, 0, Position.z + ZoomDistance)) * Q.GetGLMatrix();
	}
	
	return ViewMatrix;
}
