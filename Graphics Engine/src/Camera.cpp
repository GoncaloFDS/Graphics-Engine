#include "Camera.h"
#include "MatrixFactory.h"
#include <GL/glew.h>
#include "Application.h"
#include "Quat.h"

float radians(const float degrees) {
	return degrees * PI / 180;
}

Camera::Camera(const Vec3 pos, const Vec3 center) {
	position = pos;
	movementSpeed = 3.f;
	mouseSensivity = 0.005f;
	rotationAngle = { 0, 0, 0 };
	rotationDelta = { 0, 0, 0 };
	viewMatrix = MatrixFactory::Translate(Vec3(0, 0, -15));
	zoomDistance = 0;
	usingQuaternions = false;
	q = Quat(1, 0, 0, 0);
}


void Camera::setLeftButton(const bool b) {
	leftButtonPressed = b;
}

void Camera::setIsFirstMouseInput(const bool b) {
	firstMouseInput = b;
}

void Camera::moveCamera(const movementDir dir, const float deltaTime) {
	const float velocity = movementSpeed * deltaTime;
	if (dir == Forward)
		zoomDistance += velocity;
	if (dir == Backward)
		zoomDistance -= velocity;
	if (dir == Up) {
		rotationAngle.z += velocity;
		rotationDelta.z = velocity;
	}
	if (dir == Down) {
		rotationAngle.z -= velocity;
		rotationDelta.z = -velocity;
	}
}

void Camera::moveMouse(int x, int y, Vec2 screen) {
	if (leftButtonPressed) {
		if (firstMouseInput) {
			lastMouse.x = x;
			lastMouse.y = y;
			firstMouseInput = false;
		}

		rotationAngle.x += (lastMouse.x - x) * mouseSensivity;
		rotationAngle.y += (lastMouse.y - y) * mouseSensivity;

		rotationDelta.x = (lastMouse.x - x) *0.5f ;
		rotationDelta.y = (lastMouse.y - y) *0.5f;

		lastMouse.x = x;
		lastMouse.y = y;

	}
}

Mat4 Camera::getViewMatrix() {
	if(!usingQuaternions)
		viewMatrix = MatrixFactory::Translate(Vec3(0, 0, -10 + zoomDistance)) * 
		MatrixFactory::Rotate(rotationAngle.x, Vec3(0, 1, 0)) * 
		MatrixFactory::Rotate(rotationAngle.y, Vec3(1, 0, 0)) * 
		MatrixFactory::Rotate(rotationAngle.z, Vec3(0, 0, 1));
	else {
		q = Quat(rotationDelta.x, Vec4(0, 1, 0, 1)) * 
			Quat(rotationDelta.y, Vec4(1, 0, 0, 1)) * 
			Quat(rotationDelta.z * 50, Vec4(0, 0, 1, 1)) * q;
		rotationDelta = { 0, 0, 0 };
		viewMatrix = MatrixFactory::Translate(Vec3(0, 0, -10 + zoomDistance)) * q.GetGLMatrix();
	}
	
	return viewMatrix;
}
