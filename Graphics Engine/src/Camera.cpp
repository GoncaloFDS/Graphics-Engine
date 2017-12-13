#include "Camera.h"
#include "MatrixFactory.h"
#include <GL/glew.h>
#include "Engine.h"
#include "Quat.h"

float radians(const float degrees) {
	return degrees * PI / 180;
}

Camera::Camera(const Vec3 pos, const Vec3 center) {
	position = pos;
	front = center - position;
	worldUp = Vec3(0, 1, 0);
	right = front.Cross(worldUp);
	up = right.Cross(front);
	front.Normalize();
	right.Normalize();
	up.Normalize();
	cubeMat = MatrixFactory::identity();
	movementSpeed = 3.f;
	mouseSensivity = 0.002f;
}

Mat4 Camera::getProjectionMatrix() {
	return projMat;
}

void Camera::setProjectionMatrix(const Mat4& mat4) {
	projMat = mat4;
}

void Camera::invertPitch() {
	front.z = -front.z;
	up.y = -up.y;
}


Vec3 Camera::getFrontVector() {
	return front;
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
		position += front * velocity;
	if (dir == Backward)
		position -= front * velocity;
	if (dir == Right)
		position += right * velocity;
	if (dir == Left)
		position -= right * velocity;
	if (dir == Up)
		position += up * velocity;
	if (dir == Down)
		position -= up * velocity;
}

void Camera::moveMouse(int x, int y, Vec2 screen) {
	if (leftButtonPressed) {
		if (firstMouseInput) {
			lastMouse.x = x;
			lastMouse.y = y;
			firstMouseInput = false;
		}

		Vec2 offset(lastMouse.x - x, lastMouse.y - y);
		lastMouse.x = x;
		lastMouse.y = y;

		offset.x *= mouseSensivity;
		offset.y *= mouseSensivity;
		pitch += offset.y;
		Mat4 rotator = MatrixFactory::rotate(offset.y, right) *  MatrixFactory::rotate(offset.x, up);
		//cubeMat = rotator * cubeMat;
		front = Mat3(rotator) * front;
		up = Mat3(rotator) * up;
		up.x = 0;
		right = front.Cross(up);
		front.Normalize();
		right.Normalize();
		up.Normalize();
	}
}


Mat4 Camera::getViewMatrix() {
	return MatrixFactory::lookAt(position, position + front, up);
	
}
