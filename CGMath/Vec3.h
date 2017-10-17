#pragma once
#include <iostream>

struct Vec3{
	float x, y, z;
	Vec3();
	Vec3(float xa, float ya, float za);
	~Vec3();

	friend std::ostream& operator<<(std::ostream& os, const Vec3&);
	friend std::istream& operator>>(std::istream& is, Vec3& vector);

	bool operator==(Vec3& other) const;
	bool operator!=(Vec3 & other) const;
	Vec3& operator=(const Vec3& other);

	Vec3& operator+=(const Vec3& other);
	Vec3 operator+(const Vec3& other) const;
	Vec3& operator-=(const Vec3& other);
	Vec3 operator-(const Vec3& other) const;

	Vec3& operator*=(const float& scalar);
	Vec3 operator*(const float& scalar) const;
	Vec3& operator/=(const float& scalar);
	Vec3 operator/(const float& scalar) const;

	float Quadrance() const;
	float Magnitude() const;
	Vec3& Normalize();

	float Dot(const Vec3& other) const;
	Vec3 Cross(const Vec3& other) const;
};

Vec3 operator*(const float& scalar, const Vec3& vector);
