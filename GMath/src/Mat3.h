#pragma once
#include "Vec3.h"
#include "Mat4.h"

struct Mat4;

struct Mat3 {
	float entry[9] = {0};

	Mat3() = default;
	Mat3(
		float e0, float e1, float e2, 
		float e3, float e4, float e5, 
		float e6, float e7, float e8
	);
	Mat3(const float e[9]);
	Mat3(const float diagonal);
	Mat3(const Mat4& m);
	bool operator==(Mat3& other) const;
	bool operator!=(Mat3& other) const;
	void Clean();

	float Get(int i, int j);
	friend std::ostream& operator<<(std::ostream& os, const Mat3& m);

	float Determinant();

	friend Mat3 operator*(Mat3 left, const Mat3& right);
	Mat3 operator*=(const Mat3& other);
	
	friend Mat3 operator+(Mat3 left, const Mat3& right);
	Mat3 operator+=(const Mat3& other);
	
	friend Mat3 operator-(Mat3 left, const Mat3& right);
	Mat3 operator-=(const Mat3& other);

	friend Mat3 operator*(const Mat3 left, const float scalar);
	Mat3 operator*=(const float scalar);

	friend Vec3 operator*(const Mat3& left, const Vec3& right);
};
