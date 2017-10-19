#include "Vec4.h"
#include "MathExceptions.h"

Vec4::Vec4(float a, float b, float c, float d) 
	: x(a), y(b), z(c), w(d) {}

Vec4::Vec4(const Vec3& v, float d) 
	: x(v.x), y(v.y), z(v.z), w(w) {}

float* Vec4::asArray() const {
	float v[4] = { x, y, z, w };
	return v;
}

bool Vec4::operator==(const Vec4& other) const {
	return x == other.x && y == other.y && z == other.z && w == other.w;
}

bool Vec4::operator!=(const Vec4& other) const {
	return !(*this == other);
}

Vec4 operator+(Vec4 left, const Vec4& right) {
	return left.Add(right);
}

Vec4 operator-(Vec4 left, const Vec4& right) {
	return left.Subtract(right);
}

Vec4 operator*(Vec4 left, const Vec4& right) {
	return left.Multiply(right);
}

Vec4& Vec4::operator+=(const Vec4& other) {
	return Add(other);
}

Vec4& Vec4::operator-=(const Vec4& other) {
	return Subtract(other);
}

Vec4& Vec4::operator*=(const Vec4& other) {
	return Multiply(other);
}


Vec4& Vec4::operator*=(const float& scalar) {
	return Multiply(scalar);
}

Vec4 operator*(Vec4 left, const float& scalar) {
	return left.Multiply(scalar);
}

Vec4 operator/(Vec4 left, const float& scalar) {
	return left.Divide(scalar);
}

Vec4& Vec4::operator/=(const float& scalar) {
	return Divide(scalar);
}


float Vec4::Dot(const Vec4& other) const {
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

std::ostream& operator<<(std::ostream& stream, const Vec4& vector) {
	stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
	return stream;
}

Vec4& Vec4::Add(const Vec4& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

Vec4& Vec4::Subtract(const Vec4& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

Vec4& Vec4::Multiply(const Vec4& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;
	return *this;
}

Vec4& Vec4::Multiply(const float& scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

Vec4& Vec4::Divide(const float& scalar) {
	if (scalar == 0) throw VectorDividedByZero();
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
	return *this;
}