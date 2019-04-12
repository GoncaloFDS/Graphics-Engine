#pragma once
#ifndef QUATERNIONCAMERA_H
#define QUATERNIONCAMERA_H
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Camera.h"


class QuaternionCamera
{
public:
	QuaternionCamera();
	QuaternionCamera(Vector3 distance);
	void ChangeOrientation(Quaternion q1);
	void ChangeOrientation(float Angle, Vector4 Axis);
	void MoveCamera(Direction dir, float deltaTime);
	void setCameraSpeed(float newSpeed);
	void setRotationFactor(float newRotation);
	void setViewMatrix(Matrix4 newView);
	void setProjectionMatrix(Matrix4 newProjection);
	Matrix4 GetProjectionMatrix();
	Matrix4 GetCamera();
	void UpdateCamera();
	float CameraSpeed = 0.5f;
	float rotationFactor = 0.25f;
	~QuaternionCamera();
private:
	Quaternion Q;
	Vector3 distanceToOrigin;
	Matrix4 currentCamera;
	Matrix4 currentProjection = Matrix4::ProjectionMatrix(30.0f, 640.0f / 480.0f, 1, 10);
	Quaternion RotationBetweenVectors(Vector3 start, Vector3 dest);
	

};

#endif