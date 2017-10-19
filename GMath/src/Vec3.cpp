#include "Vec3.h"
#include "MathExceptions.h"

Vec3::Vec3() = default;

Vec3::Vec3(const float xa, const float ya, const float za) : 
	x(xa), y(ya), z(za) {}

Vec3::~Vec3() = default;

std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}

std::istream& operator>>(std::istream& is, Vec3& vector) {
		is.ignore(1, '(');
		is >> vector.x;
		is.ignore(2, ' ');
		is >> vector.y;
		is.ignore(2, ' ');
		is >> vector.z;
		is.ignore(10, '\n');
		return is;
}

bool Vec3::operator==(Vec3& other) const {
	return x == other.x && y == other.y && z == other.z;
}

bool Vec3::operator!=(Vec3& other) const {
	return !(*this == other);
}

Vec3& Vec3::operator=(const Vec3& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

Vec3& Vec3::operator+=(const Vec3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vec3 Vec3::operator+(const Vec3& other) const {
	return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3& Vec3::operator-=(const Vec3& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vec3 Vec3::operator-(const Vec3& other) const {
	return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3& Vec3::operator*=(const float& scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Vec3 Vec3::operator*(const float& scalar) const {
	return Vec3(x*scalar, y*scalar, z*scalar);
}

Vec3 operator*(const float& scalar, const Vec3& vector) {
	return Vec3(vector.x*scalar, vector.y*scalar, vector.z*scalar);
}


Vec3& Vec3::operator/=(const float& scalar) {
	if (scalar == 0) throw VectorDividedByZero();
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

Vec3 Vec3::operator/(const float& scalar) const {
	if (scalar == 0) throw VectorDividedByZero();
	return Vec3(x / scalar, y / scalar, z / scalar);
}

float Vec3::Quadrance() const {
	return x*x + y*y + z*z;
}

float Vec3::Magnitude() const {
	return sqrt(Quadrance());
}

Vec3& Vec3::Normalize() {
	const float magnitude = this->Magnitude();
	if (magnitude == 0) throw NormalizeZeroVetor();
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
	return *this;
}

float Vec3::Dot(const Vec3& other) const {
	return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::Cross(const Vec3& other) const {
	return Vec3(y*other.z - z*other.y,
		z*other.x - x*other.z,
		x*other.y - y *other.x);
}


