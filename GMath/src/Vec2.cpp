#include "Vec2.h"
#include <tuple>

Vec2::Vec2() = default;

Vec2::Vec2(const float xa, const float ya) : x(xa), y(ya) {}

Vec2::~Vec2() = default;

std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
}

std::istream& operator>>(std::istream& is, Vec2& vector) {
	is.ignore(1, '(');
	is >> vector.x;
	is.ignore(2, ' ');
	is >> vector.y;
	is.ignore(10, '\n');
	return is;
}

bool Vec2::operator==(Vec2& other) const {
	return x == other.x && y == other.y;
}

bool Vec2::operator!=(Vec2& other) const {
	return !(*this == other);
}

Vec2& Vec2::operator=(const Vec2& other) {
	x = other.x;
	y = other.y;
	return *this;
}


Vec2& Vec2::operator+=(const Vec2 & other) {
	x += other.x;
	y += other.y;
	return *this;
}

Vec2 Vec2::operator+(const Vec2& other) const {
	return Vec2(x + other.x, y + other.y);
}

Vec2& Vec2::operator-=(const Vec2& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

Vec2 Vec2::operator-(const Vec2& other) const {
	return Vec2(x - other.x, y - other.y);
}

Vec2& Vec2::operator*=(const float& scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

Vec2 Vec2::operator*(const float& scalar) const {
	return Vec2(x*scalar, y*scalar);
}

Vec2 operator*(const float& scalar, const Vec2& vector) {
	return Vec2(vector.x*scalar, vector.y*scalar);
}

Vec2& Vec2::operator/=(const float& scalar) {
	x /= scalar;
	y /= scalar;
	return *this;
}

Vec2 Vec2::operator/(const float& scalar) const {
	return Vec2(x / scalar, y / scalar);
}

float Vec2::Quadrance() const {
	return x*x + y*y;
}

float Vec2::Magnitude() const {
	return sqrt(Quadrance());
}

Vec2& Vec2::Normalize() {
	//if (*this == Vec2(0, 0)) throw NormalizeZeroVetor();
	const float magnitude = this->Magnitude();
	x /= magnitude;
	y /= magnitude;
	return *this;
}

float Vec2::Dot(const Vec2& other) const {
	return x * other.x + y * other.y;
}

