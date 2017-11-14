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
	rotationDeltaR = { 0, 0, 0 };
	rotationDeltaQ = { 0, 0, 0 };
	viewMatrix = MatrixFactory::Translate(pos);
	zoomDistance = 0;
	usingQuaternions = false;
	q = Quat(1, 0, 0, 0);
	r = MatrixFactory::Identity();
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
		rotationDeltaR.z += velocity;
		rotationDeltaQ.z = velocity;
	}
	if (dir == Down) {
		rotationDeltaR.z -= velocity;
		rotationDeltaQ.z = -velocity;
	}
}

void Camera::moveMouse(int x, int y, Vec2 screen) {
	if (leftButtonPressed) {
		if (firstMouseInput) {
			lastMouse.x = x;
			lastMouse.y = y;
			firstMouseInput = false;
		}

		rotationDeltaR.x += (lastMouse.x - x) * mouseSensivity;
		rotationDeltaR.y += (lastMouse.y - y) * mouseSensivity;

		rotationDeltaQ.x = (lastMouse.x - x) *0.5f ;
		rotationDeltaQ.y = (lastMouse.y - y) *0.5f;

		lastMouse.x = x;
		lastMouse.y = y;

	}
}

Mat4 Camera::getViewMatrix() {
	if(!usingQuaternions) {
		r = MatrixFactory::Rotate(rotationDeltaR.x, Vec3(0, 1, 0)) *
			MatrixFactory::Rotate(rotationDeltaR.y, Vec3(1, 0, 0)) *
			MatrixFactory::Rotate(rotationDeltaR.z, Vec3(0, 0, 1)) ;
		//rotationDeltaR = { 0, 0, 0 };

		viewMatrix = MatrixFactory::Translate(Vec3(0, 0, position.z + zoomDistance)) * r;

	}
	else {
		q = Quat(rotationDeltaQ.x, Vec4(0, 1, 0, 1)) * 
			Quat(rotationDeltaQ.y, Vec4(1, 0, 0, 1)) * 
			Quat(rotationDeltaQ.z * 50, Vec4(0, 0, 1, 1)) * q;
		rotationDeltaQ = { 0, 0, 0 };
		viewMatrix = MatrixFactory::Translate(Vec3(0, 0, position.z + zoomDistance)) * q.GetGLMatrix();
	}
	
	return viewMatrix;
}
