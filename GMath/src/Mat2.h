#pragma once
#include "Vec2.h"

struct Mat2 {
	float entry[4] = {0};

	Mat2() = default;
	Mat2(
		float e0, float e1, 
		float e2, float e3
	);
	Mat2(const float e[4]);
	Mat2(const float diagonal);

	bool operator==(Mat2 & other) const;
	bool operator!=(Mat2 & other) const;

	float Get(int i, int j);

	static Mat2 Identity();
	static Mat2 Rotate(float angle);
	static Mat2 Scale(const Vec2& s);

	static Mat2 Transpose(const Mat2& matrix);
	static Mat2 Inverse(Mat2& matrix);

	float Determinant();

	friend Mat2 operator*(Mat2 left, const Mat2& right);
	Mat2 operator*=(const Mat2& other);

	friend Mat2 operator+(Mat2 left, const Mat2& right);
	Mat2 operator+=(const Mat2& other);

	friend Mat2 operator-(Mat2 left, const Mat2& right);
	Mat2 operator-=(const Mat2& other);

	friend Mat2 operator*(const Mat2 left, const float scalar);
	Mat2 operator*=(const float scalar);

	friend Vec2 operator*(const Mat2& left, const Vec2& right);
};
