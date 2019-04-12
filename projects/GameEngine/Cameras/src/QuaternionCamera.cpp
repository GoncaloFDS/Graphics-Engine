#include "QuaternionCamera.h"



QuaternionCamera::QuaternionCamera()
{
	this->Q = Quaternion(1, 0, 0, 0);
	this->distanceToOrigin = Vector3(0, 0, 0);
}



QuaternionCamera::QuaternionCamera(Vector3 distance)
{
	this->Q = Quaternion(1,0,0,0);
	this->distanceToOrigin = distance;
	this->Q = this->Q.NormalizeQuaternion();
	/*Vector3 xyz = Vector3(distance.coordinates[0], distance.coordinates[1], distance.coordinates[2]).normalize();
	Vector3 z = Vector3(0, 0, 1);

	Vector3 rotationVector = Vector3::crossProduct(xyz, z);
	float rotation_angle = acosf(xyz*z);

	Quaternion q = Quaternion::FromAngleAxis(rotation_angle, rotationVector);

	this->Q = q * this->Q;
	
	this->distanceToOrigin = z;*/

	std::cout << this->Q << std::endl;
	std::cout << Matrix4::TRANSLATE(this->distanceToOrigin.coordinates[0], this->distanceToOrigin.coordinates[1], this->distanceToOrigin.coordinates[2])*  this->Q.ToGLMatrix() << std::endl;
	this->currentCamera = Matrix4::TRANSLATE(distance.coordinates[0], distance.coordinates[1], distance.coordinates[2]) * Q.ToGLMatrix();
	std::cout << currentCamera << std::endl;
	
}

void QuaternionCamera::ChangeOrientation(Quaternion q1)
{
	this->Q = (q1.NormalizeQuaternion() * this->Q).NormalizeQuaternion();
	
	this->UpdateCamera();
}

void QuaternionCamera::ChangeOrientation(float Angle, Vector4 Axis)
{
	Quaternion tempQuat = Quaternion::FromAngleAxis(Angle, Axis).NormalizeQuaternion();
	this->Q = (tempQuat * this->Q).NormalizeQuaternion();
	
//	std::cout << tempQuat << std::endl;
//	std::cout << this->Q << std::endl;
	this->UpdateCamera();
}

void QuaternionCamera::MoveCamera(Direction dir, float deltaTime)
{
	switch(dir)
	{
	case Up:
		this->distanceToOrigin += Vector3(0, 1, 0) * deltaTime * this->CameraSpeed;
		break;
	case Down:
		this->distanceToOrigin -= Vector3(0, 1, 0) * deltaTime * this->CameraSpeed;
		break;
	case Forward:
		this->distanceToOrigin += Vector3(0, 0, 1) * deltaTime * this->CameraSpeed;
		break;
	case Backward:
		this->distanceToOrigin -= Vector3(0, 0, 1) * deltaTime * this->CameraSpeed;
		break;
	case Left:
		this->distanceToOrigin += Vector3(1, 0, 0) * deltaTime * this->CameraSpeed;
		break;
	case Right:
		this->distanceToOrigin -= Vector3(1, 0, 0) * deltaTime * this->CameraSpeed;
		break;
	}
	this->UpdateCamera();
}

void QuaternionCamera::setCameraSpeed(float newSpeed)
{
	this->CameraSpeed = newSpeed;
}

void QuaternionCamera::setRotationFactor(float newRotation)
{
	this->rotationFactor = newRotation;
}

void QuaternionCamera::setViewMatrix(Matrix4 newView)
{
	this->currentCamera = newView;
}

void QuaternionCamera::setProjectionMatrix(Matrix4 newProjection)
{
	this->currentProjection = newProjection;
}

Matrix4 QuaternionCamera::GetProjectionMatrix()
{
	return this->currentProjection;
}

Matrix4 QuaternionCamera::GetCamera()
{
//	std::cout << this->Q << std::endl;
//	std::cout << this->distanceToOrigin << std::endl;
	return this->currentCamera;
	//return Matrix4::TRANSLATE(this->distanceToOrigin.coordinates[0], this->distanceToOrigin.coordinates[1], this->distanceToOrigin.coordinates[2]) * this->Q.QuaternionToGLMatrix();
}

void QuaternionCamera::UpdateCamera()
{
	this->currentCamera = Matrix4::TRANSLATE(this->distanceToOrigin.coordinates[0], this->distanceToOrigin.coordinates[1], this->distanceToOrigin.coordinates[2]) * this->Q.ToGLMatrix();
}


QuaternionCamera::~QuaternionCamera()
{
}

Quaternion QuaternionCamera::RotationBetweenVectors(Vector3 start, Vector3 dest)
{
	start = start.normalize();
	dest = dest.normalize();
	float cosTheta = start*dest;
	Vector3 rotationAxis;
	if (cosTheta < -1 + 0.001f) {
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = Vector3::crossProduct(Vector3(0.0f, 0.0f, 1.0f), start);
		if (rotationAxis.magnitude() < 0.01) // bad luck, they were parallel, try again!
			rotationAxis = Vector3::crossProduct(Vector3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = rotationAxis.normalize();
		return Quaternion::FromAngleAxis(180.0f, rotationAxis).NormalizeQuaternion();
	}
	else {
		rotationAxis = Vector3::crossProduct(start, dest);
		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;
		return Quaternion(s*0.5f, rotationAxis.coordinates[0] * invs,
			rotationAxis.coordinates[1] * invs, rotationAxis.coordinates[2] * invs).NormalizeQuaternion();
	}
}
