#pragma once
#include "Vec3.h"
#include "Mat3.h"

struct Mat3;

struct Mat4 {

	float entry[16] = {0}; 

	Mat4() = default;
	Mat4(float e0, float e1, float e2, float e3,
		float e4, float e5, float e6, float e7,
		float e8, float e9, float e10, float e11,
		float e12, float e13, float e14, float e15);
	Mat4(const float[16]);
	Mat4(float diagonal);
	Mat4(const Mat3& m);
	~Mat4();
	Mat4 rowMajor() const;

	float get(int i, int j);
	friend std::ostream& operator<<(std::ostream& os, const Mat4& m);
	
	bool operator==(Mat4& other) const;
	bool operator!=(Mat4& other) const;

	void clean();

	friend Mat4 operator*(Mat4 left, const Mat4& right);
	Mat4& operator*=(const Mat4& other);
	
	friend Mat4 operator+(Mat4 left, const Mat4& right);
	Mat4& operator+=(const Mat4& other);
	
	friend Mat4 operator-(Mat4 left, const Mat4& right);
	Mat4& operator-=(const Mat4& other);

	friend Vec3 operator*(const Mat4& left, const Vec3& right);

	float determinant();

	friend Mat4 operator*(const Mat4 left, const float scalar);
	Mat4 operator*=(const float scalar);

};

