#include "Quat.h"

Quat::Quat(float t, float x, float y, float z) : t(t), x(x), y(y), z(z)
{}

Quat::Quat(float theta, Vec4 axis) {
	axis.Normalize();
	float a = theta * DEGREES_TO_RADIANS;
	t = cos(a / 2.0f);
	float s = sin(a / 2.0f);
	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
	Clean();
	Normalize();
}

const void Quat::Clean() {
	if (fabs(t) < CloseToZero) t = 0.0f;
	if (fabs(x) < CloseToZero) x = 0.0f;
	if (fabs(y) < CloseToZero) y = 0.0f;
	if (fabs(z) < CloseToZero) z = 0.0f;
}

Quat Quat::Normalize() const {
	return *this*(1 / Magnitude());
}

const void Quat::ToAngleAxis(float& theta, Vec4& axis) {
	Normalize();
	theta = 2.0f * acos(t) * RADIANS_TO_DEGREES;
	float s = sqrt(1.0f - t*t);
	if (s < CloseToZero) {
		axis.x = 1.0f;
		axis.y = 0.0f;
		axis.z = 0.0f;
		axis.w = 1.0f;
	}
	else {
		axis.x = x / s;
		axis.y = y / s;
		axis.z = z / s;
		axis.w = 1.0f;
	}
}

const float Quat::Quadrance() const {
	return t*t + x*x + y*y + z*z;
}

const float Quat::Magnitude() const {
	return sqrt(Quadrance());
}

const Quat Quat::Conjugate() const {
	return Quat{ t, -x, -y, -z };
}

Quat Quat::operator+(const Quat q) const {
	return Quat {
		t + q.t,
		x + q.x,
		y + q.y,
		z + q.z,
	};
}

Quat Quat::operator-(const Quat q) const {
	return Quat{
		t - q.t,
		x - q.x,
		y - q.y,
		z - q.z,
	};
}

Quat Quat::operator*(const Quat q) const {
	return Quat {
		t * q.t - x * q.x - y * q.y - z * q.z,
		t * q.x + x * q.t + y * q.z - z * q.y,
		t * q.y + y * q.t + z * q.x - x * q.z,
		t * q.z + z * q.t + x * q.y - y * q.x,
	};
}

Quat Quat::operator*(const float s) const {
	return Quat{ t*s, x*s, y*s, z*s };
}

Mat4 Quat::GetGLMatrix() {
	Normalize();
	float xx = x * x;
	float xy = x * y;
	float xz = x * z;
	float xt = x * t;
	float yy = y * y;
	float yz = y * z;
	float yt = y * t;
	float zz = z * z;
	float zt = z * t;
	Mat4 matrix;
	matrix.entry[0] = 1.0f - 2.0f * (yy + zz);
	matrix.entry[1] = 2.0f * (xy + zt);
	matrix.entry[2] = 2.0f * (xz - yt);
	matrix.entry[3] = 0.0f;

	matrix.entry[4] = 2.0f * (xy - zt);
	matrix.entry[5] = 1.0f - 2.0f * (xx + zz);
	matrix.entry[6] = 2.0f * (yz + xt);
	matrix.entry[7] = 0.0f;

	matrix.entry[8] = 2.0f * (xz + yt);
	matrix.entry[9] = 2.0f * (yz - xt);
	matrix.entry[10] = 1.0f - 2.0f * (xx + yy);
	matrix.entry[11] = 0.0f;

	matrix.entry[12] = 0.0f;
	matrix.entry[13] = 0.0f;
	matrix.entry[14] = 0.0f;
	matrix.entry[15] = 1.0f;

	matrix.Clean();
	return matrix;
}

Quat Quat::Lerp(const Quat target, float k) {
	float cos_angle = x*target.x + y*target.y + z*target.z + t*target.t;
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;
	Quat qi = (*this)*k0 + target*k1;
	qi.Normalize();
	return qi;
}

Quat Quat::Slerp(const Quat target, float k) {
	float angle = acos(x*target.x + y*target.y + z*target.z + t*target.t);
	float k0 = sin((1 - k)*angle) / sin(angle);
	float k1 = sin(k*angle) / sin(angle);
	Quat qi = (*this)*k0 + target*k1;
	qi.Normalize();
	return qi;
}

Quat operator*(const float s, const Quat q) {
	return q * s;
}
