#include "Camera.h"
#include "MatrixFactory.h"
#include <GL/glew.h>
#include "Application.h"

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

	movementSpeed = 3.f;
	mouseSensivity = 0.002f;
}


void Camera::setLeftButton(bool b) {
	leftButtonPressed = b;
}

void Camera::setIsFirstMouseInput(bool b) {
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

void Camera::updateVectors() {
	//right = front.Cross(worldUp);
	//up = right.Cross(front);
	front.Normalize();
	right.Normalize();
	up.Normalize();

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

		Mat4 rotator = MatrixFactory::Rotate(offset.y, right) *  MatrixFactory::Rotate(offset.x, up);
		front = Mat3(rotator) * front;
		//Mat4 upRotator = MatrixFactory::Rotate(offset.y, right) ;
		up = Mat3(rotator) * up;
		up.x = 0;
		right = front.Cross(up);

		std::cout << "front " << front << std::endl;
		std::cout << "right " << right << std::endl;
		std::cout << "up " << up << std::endl;

		updateVectors();
	}
}

Mat4 Camera::getViewMatrix() const {
	return MatrixFactory::LookAt(position, position + front, up);
}


