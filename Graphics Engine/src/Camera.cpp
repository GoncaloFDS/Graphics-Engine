#include "Camera.h"
#include "MatrixFactory.h"
#include <GL/glew.h>
#include "Application.h"
#include <valarray>

float radians(float degrees) {
	return degrees * PI / 180;
}

Camera::Camera(Vec3 pos) {
	position = pos;
	front = Vec3(0, 0, 0) - position;
	frontDist = front.Magnitude();
	worldUp = Vec3(0, 1, 0);
	right = front.Cross(worldUp);
	up = right.Cross(front);
	front.Normalize();
	right.Normalize();
	up.Normalize();
	
	yaw = -acos((Vec3(front.x, 0, front.z).Normalize()).Dot(Vec3(0, 0, -1))) *180.0f / PI;
	pitch = -acos((Vec3(0, front.y, front.z).Normalize()).Dot(Vec3(0, 0, -1))) *180.0f / PI;
	movementSpeed = 3.f;
	mouseSensivity = 0.1f;

	/*std::cout << front << std::endl;
	std::cout << "yaw :: " << yaw << std::endl;
	std::cout << "pitch ::" << pitch << std::endl;
	std::cout << "position ::" << position << std::endl;
*/
}


void Camera::setLeftButton(bool b) {
	leftButtonPressed = b;
}

void Camera::setIsFirstMouseInput(bool b) {
	firstMouseInput = b;
}

void Camera::moveCamera(movementDir dir, float deltaTime) {
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
	front.x = sin(radians(yaw)) * cos(radians(pitch));
	front.y = sin(radians(pitch));
	front.z = -cos(radians(pitch)) * cos(radians(yaw));
	/*std::cout << front << std::endl;
	std::cout << "yaw :: " << yaw << std::endl;
	std::cout << "pitch ::" << pitch << std::endl;
	std::cout << "position ::" << position << std::endl;*/
	right = front.Cross(worldUp);
	up = right.Cross(front);
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

		Vec2 offset(x - lastMouse.x, lastMouse.y - y);
		lastMouse.x = x;
		lastMouse.y = y;

		offset.x *= mouseSensivity;
		offset.y *= mouseSensivity;

		yaw += offset.x;
		pitch += offset.y;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		updateVectors();
	}
}

Mat4 Camera::getViewMatrix() {
	return MatrixFactory::LookAt(position, position + front, up);
}


