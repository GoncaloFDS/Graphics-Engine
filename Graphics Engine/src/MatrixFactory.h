#pragma once
#include "Vec3.h"
#include "Mat4.h"

class MatrixFactory {
public:
	MatrixFactory();
	~MatrixFactory();

	static Mat4 Identity();
	static Mat4 Translate(const Vec3& translation);
	static Mat4 Rotate(const float angle, Vec3 axis);
	static Mat4 Scale(const Vec3& scale);

	static Mat4 Transpose(const Mat4& matrix);

	static Mat4 CrossMatrix(const Vec3& v);
	static Mat4 SqrCrossMatrix(const Vec3& v);

	static Mat4 Ortho(float l, float r, float t, float b, float n, float f);
	static Mat4 Perspective(float fov, float ratio, float n, float f);

	static Mat4 LookAt(Vec3 eye, Vec3 center, Vec3 up);
};
