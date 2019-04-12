#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "Matrix3.h"

enum Direction {
	Right, Left, Up, Down, Forward, Backward
};
class Camera
{

public:
	Vector3 currentEye;
	Vector3 currentCenter;
	Vector3 currentUp;
	Vector3 position;
	Vector3 front;
	Vector3 right;
	void Pitch(float angle);
	void Yaw(float angle);
	float xrot;
	float yrot;
	float cameraSpeed = 0.5f;
	Camera();
	Camera(Vector3 position);
	Camera(Vector3 position, Vector3 center);
	Camera(Vector3 center, Vector3 eye, Vector3 up);
	void MoveCamera(Direction dir, float velocity);
	void LookAt(float yaw, float pitch);
	Matrix4 ProjectionMatrix;
	void setProjectionMatrix(Matrix4 projection);
	Matrix4 GetProjectionMatrix();
	Matrix4 GetCamera();
	~Camera();
	void invertPitch();
};

