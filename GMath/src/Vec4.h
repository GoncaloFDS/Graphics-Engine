#pragma once
#include "Vec3.h"

struct Vec4 {
	float x, y, z, w;
	
	Vec4() = default;
	~Vec4() = default;
	Vec4(float a, float b, float c, float d);
	Vec4(const Vec3& v,  float d);

	float* asArray() const;

	bool operator==(const Vec4& other) const;
	bool operator!=(const Vec4& other) const;

	friend Vec4 operator+(Vec4 left, const Vec4& right);
	friend Vec4 operator-(Vec4 left, const Vec4& right);
	friend Vec4 operator*(Vec4 left, const Vec4& right);

	Vec4& operator+=(const Vec4& other);
	Vec4& operator-=(const Vec4& other);
	Vec4& operator*=(const Vec4& other);

	friend Vec4 operator*(Vec4 left, const float& scalar);
	Vec4& operator*=(const float& scalar);
	friend Vec4 operator/(Vec4 left, const float& scalar);
	Vec4& operator/=(const float& scalar);

	float Dot(const Vec4& other) const;

	friend std::ostream& operator<<(std::ostream& stream, const Vec4& vector);

private:
	Vec4& Add(const Vec4& other);
	Vec4& Subtract(const Vec4& other);
	Vec4& Multiply(const Vec4& other);
	
	Vec4& Multiply(const float& scalar);	
	Vec4& Divide(const float& scalar);	
};

