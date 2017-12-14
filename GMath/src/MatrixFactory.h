#pragma once
#include "Vec3.h"
#include "Mat4.h"

class MatrixFactory {
public:
	MatrixFactory();
	~MatrixFactory();

	static Mat4 identity();
	static Mat4 translate(const Vec3& translation);
	static Mat4 rotate(const float angle, Vec3 axis);
	static Mat4 scale(const Vec3& scale);

	static Mat4 transpose(const Mat4& matrix);
	static Mat3 inverse(Mat3& m);
	

	static Mat4 crossMatrix(const Vec3& v);
	static Mat4 sqrCrossMatrix(const Vec3& v);

	static Mat4 ortho(float l, float r, float t, float b, float n, float f);
	static Mat4 perspective(float fov, float ratio, float n, float f);

	static Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up);
};
