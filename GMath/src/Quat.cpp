#include "Quat.h"

Quat::Quat(float t_, float x_, float y_, float z_) : t(t_), x(x_), y(y_), z(z_)
{}

Quat::Quat(float theta, Vec4 axis) {
	axis.Normalize();
	float a = theta;
	t = cos(a / 2.0f);
	float s = sin(a / 2.0f);
	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
	clean();
	normalize();
}

void Quat::clean() {
	if (fabs(t) < CloseToZero) t = 0.0f;
	if (fabs(x) < CloseToZero) x = 0.0f;
	if (fabs(y) < CloseToZero) y = 0.0f;
	if (fabs(z) < CloseToZero) z = 0.0f;
}

Quat Quat::normalize() const {
	return *this*(1 / magnitude());
}

const void Quat::toAngleAxis(float& theta, Vec4& axis) const {
	normalize();
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

const float Quat::quadrance() const {
	return t*t + x*x + y*y + z*z;
}

float Quat::magnitude() const {
	return sqrt(quadrance());
}

const Quat Quat::conjugate() const {
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

Mat4 Quat::toMatrix() {
	normalize();
	float xx = x * x;
	float xy = x * y;
	float xz = x * z;
	float xt = x * t;
	float yy = y * y;
	float yz = y * z;
	float yt = y * t;
	float zz = z * z;
	float zt = z * t;
	Mat4 matrix {
		1.0f - 2.0f * (yy + zz),  2.0f * (xy - zt) , 2.0f * (xz + yt), 0,
		2.0f * (xy + zt) , 1.0f - 2.0f * (xx + zz) , 2.0f * (yz - xt), 0,
		2.0f * (xz - yt), 2.0f * (yz + xt), 1.0f - 2.0f * (xx + yy), 0,
		0, 0, 0, 1
	};

	matrix.clean();
	return matrix;
}

bool Quat::operator==(const Quat q) const {
	return (fabs(t - q.t) < CloseToZero && fabs(x - q.x) < CloseToZero &&
		fabs(y - q.y) < CloseToZero && fabs(z - q.z) < CloseToZero);
}

bool Quat::operator!=(const Quat q) const {
	return !((*this) == q);
}

Quat Quat::inverse() const {
	return conjugate() * (1.0f / quadrance());
}

Quat Quat::lerp(const Quat start, const Quat target, const float k) {
	float cos_angle = start.x*target.x + start.y*target.y + start.z*target.z + start.t*target.t;
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;
	Quat qi = start*k0 + target*k1;
	qi.normalize();
	return qi;
}

Quat Quat::slerp( Quat start,  Quat target, const float k) {
	float angle = acos(start.x*target.x + start.y*target.y + start.z*target.z + start.t*target.t);
	float k0 = sin((1 - k)*angle) / sin(angle);
	float k1 = sin(k*angle) / sin(angle);
	Quat qi = start*k0 + target*k1;
	qi.normalize();
	return qi;
}

Quat operator*(const float s, const Quat q) {
	return q * s;
}
