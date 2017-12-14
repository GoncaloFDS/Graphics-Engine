#include "MatrixFactory.h"
#include "MathExceptions.h"

MatrixFactory::MatrixFactory() {
}

MatrixFactory::~MatrixFactory() = default;

Mat4 MatrixFactory::identity() {
	return Mat4(1.f);
}

Mat4 MatrixFactory::translate(const Vec3& t) {
	return Mat4{
		1, 0, 0, t.x,
		0, 1, 0, t.y,
		0, 0, 1, t.z,
		0, 0, 0, 1
	};
}

Mat4 MatrixFactory::rotate(const float angle, Vec3 axis) {
	axis.Normalize();
	return Mat4(1.0f) + sin(angle) * crossMatrix(axis)
		+ (1 - cos(angle)) * sqrCrossMatrix(axis);
}

Mat4 MatrixFactory::scale(const Vec3& s) {
	return Mat4 {
		s.x, 0, 0, 0,
		0, s.y, 0, 0,
		0, 0, s.z, 0,
		0, 0, 0, 1
	};
}

Mat4 MatrixFactory::transpose(const Mat4& m) {
	return Mat4 {
		m.entry[0], m.entry[4], m.entry[8], m.entry[12],
		m.entry[1], m.entry[5], m.entry[9], m.entry[13],
		m.entry[2], m.entry[6], m.entry[10], m.entry[14],
		m.entry[3], m.entry[7], m.entry[11], m.entry[15],
	};
}


Mat3 MatrixFactory::inverse(Mat3& m) {
	float det = m.Determinant();
	if (det == 0) throw DerterminatEqualsZero();
	float invDet = 1 / det;
	float result[9];
	result[0] = (m.get(1, 1) * m.get(2, 2) - m.get(2, 1) * m.get(1, 2)) * invDet;
	result[3] = (m.get(1, 2) * m.get(2, 0) - m.get(1, 0) * m.get(2, 2)) * invDet;
	result[6] = (m.get(1, 0) * m.get(2, 1) - m.get(2, 0) * m.get(1, 1)) * invDet;
	result[1] = (m.get(0, 2) * m.get(2, 1) - m.get(0, 1) * m.get(2, 2)) * invDet;
	result[4] = (m.get(0, 0) * m.get(2, 2) - m.get(0, 2) * m.get(2, 0)) * invDet;
	result[7] = (m.get(2, 0) * m.get(0, 1) - m.get(0, 0) * m.get(2, 1)) * invDet;
	result[2] = (m.get(0, 1) * m.get(1, 2) - m.get(0, 2) * m.get(1, 1)) * invDet;
	result[5] = (m.get(1, 0) * m.get(0, 2) - m.get(0, 0) * m.get(1, 2)) * invDet;
	result[8] = (m.get(0, 0) * m.get(1, 1) - m.get(1, 0) * m.get(0, 1)) * invDet;
	return Mat3(result);
}

Mat4 MatrixFactory::crossMatrix(const Vec3& k) {
	return Mat4 {
		0, -k.z, k.y, 0,
		k.z, 0, -k.x, 0,
		-k.y, k.x, 0, 0,
		0, 0, 0, 0
	};
}

Mat4 MatrixFactory::sqrCrossMatrix(const Vec3& k) {
	return Mat4 {
		-(k.z * k.z) - (k.y * k.y), k.x * k.y, k.x * k.z, 0,
		k.x * k.y, -(k.z * k.z) - (k.x * k.x), k.y * k.z, 0,
		k.x * k.z, k.y * k.z, -(k.x * k.x) - (k.y * k.y), 0,
		0, 0, 0, 0
	};
}

Mat4 MatrixFactory::ortho(const float l, const float r, const float t, const float b, const float n, const float f) {
	return Mat4 {
		2.f / (r - l), 0, 0, (l + r) / (l - r),
		0, 2.f / (t - b), 0, (b + t) / (b - t),
		0, 0, 2.f / (n - f), (n + f) / (n - f),
		0, 0, 0, 1
	};
}

Mat4 MatrixFactory::perspective(const float fov, const float ratio, const float n, const float f) {
	const float d = 1.f / tan(fov / 2.f);
	return Mat4 {
		d / ratio, 0, 0, 0,
		0, d, 0, 0,
		0, 0, (n + f) / (n - f), 2.f*f*n/(n -f),
		0, 0, -1, 0
	};
}

Mat4 MatrixFactory::lookAt(const Vec3 eye, const Vec3 center, const Vec3 up) {
	Vec3 v = center - eye;
	v.Normalize();
	Vec3 s = v.Cross(up);
	s.Normalize();
	const Vec3 u = s.Cross(v);
	
	return Mat4 {
		s.x, s.y, s.z, (-1 * s).Dot(eye),
		u.x, u.y, u.z, (-1 * u).Dot(eye),
		-v.x, -v.y, -v.z, v.Dot(eye),
		0, 0, 0, 1
	};
}
