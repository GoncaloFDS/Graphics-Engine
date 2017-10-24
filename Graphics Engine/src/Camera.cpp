#include "Camera.h"
#include "MatrixFactory.h"
#include <GL/glew.h>
#include "Application.h"

float radians(float degrees) {
	return degrees * PI / 180;
}

Camera::Camera(Vec3 pos) {
	position = pos;
	right = Vec3(1, 0, 0);
	front = Vec3(0, 0, 0) - position;
	worldUp = Vec3(0, 1, 0);
	movementSpeed = 20.f;
	mouseSensivity = .00001f;
	yaw = 0;
	pitch = 0;
	
}


void Camera::moveCamera(movementDir dir, float deltaTime) {
	float velocity = movementSpeed * deltaTime;
	if (dir == Forward)
		position += front * velocity;
	if (dir == Backward)
		position -= front * velocity;
	if (dir == Right)
		position += right * velocity;
	if (dir == Left)
		position -= right * velocity;
	if (dir == Up)
		position += worldUp * velocity;
	if (dir == Down)
		position -= worldUp * velocity;
}

void Camera::updateVectors() {
	front.x = cos(radians(yaw) * cos(radians(pitch)));
	front.y = sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));
	front.Normalize();
	right = (front.Cross(worldUp)).Normalize();

}

void Camera::moveMouse(int xOffset, int yOffset) {
	yaw += xOffset * mouseSensivity;
	pitch += yOffset * mouseSensivity;

	if (pitch > 89.f)
		pitch = 89.f;
	if (pitch < -89.f)
		pitch = -89.f;

	updateVectors();
}

Mat4 Camera::getViewMatrix() {
	return MatrixFactory::LookAt(position, position + front, Vec3(0, 1, 0));
}


