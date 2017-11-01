#pragma once
#include "src/Vec4.h"
#include "src/Mat4.h"

#define DEGREES_TO_RADIANS (float)0.01745329251994329547
#define RADIANS_TO_DEGREES (float)57.29577951308232185913
const float CloseToZero = 1.0e-5;

class Quat {
private:
	float t, x, y, z;
public:
	Quat() = default;
	~Quat() = default;

	Quat(float t, float x, float y, float z);
	Quat(float theta, Vec4 axis);
	const void Clean();
	Quat Normalize() const;
	const void ToAngleAxis(float& theta, Vec4& axis);
	const float Quadrance() const;
	const float Magnitude() const;
	const Quat Conjugate() const;

	Quat operator+(const Quat q) const;
	Quat operator-(const Quat q) const;
	Quat operator*(const Quat q) const;
	Quat operator*(const float s) const;
	friend Quat operator*(const float s, const Quat q);
	Mat4 GetGLMatrix();

	Quat Lerp(const Quat target, float k);
	Quat Slerp(const Quat target, float k);
};

