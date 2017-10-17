#pragma once
#include <iostream>

struct Vec2 {
	float x, y;
	Vec2();
	Vec2(float xa, float ya);
	~Vec2();

	friend std::ostream& operator<<(std::ostream& os, const Vec2& vector);
	friend std::istream &operator>>(std::istream& is, Vec2& vector);

	bool operator==(Vec2& other) const;
	bool operator!=(Vec2& other) const;
	Vec2& operator=(const Vec2& other);

	Vec2& operator+=(const Vec2& other);
	Vec2 operator+(const Vec2& other) const;
	Vec2& operator-=(const Vec2& other);
	Vec2 operator-(const Vec2& other) const;

	Vec2& operator*=(const float& scalar);
	Vec2 operator*(const float& scalar) const;
	friend Vec2 operator*(const float& scalar, const Vec2& vector);
	Vec2& operator/=(const float& scalar);
	Vec2 operator/(const float& scalar) const;

	float Quadrance() const;
	float Magnitude() const;
	Vec2& Normalize();

	float Dot(const Vec2& other) const;

};
