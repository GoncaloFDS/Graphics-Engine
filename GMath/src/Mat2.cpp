#include "Mat2.h"
#include "MathExceptions.h"

Mat2::Mat2(float e0, float e1, float e2, float e3) 
	: entry {
	e0, e1,
	e2, e3 }
{}

Mat2::Mat2(const float e[4]) {
	for (int i = 0; i < 4; i++)
		entry[i] = e[i];
}

Mat2::Mat2(const float diagonal) {
	entry[0] = diagonal;
	entry[3] = diagonal;
}

bool Mat2::operator==(Mat2& other) const {
	for (int i = 0; i < 9; i++)
		if (abs(entry[i] - other.entry[i]) > CloseTozero)
			return false;
	return true;
}

bool Mat2::operator!=(Mat2& other) const {
	return !(*this == other);
}

float Mat2::Get(int i, int j) {
	return entry[i + j * 2];
}

Mat2 Mat2::identity() {
	return Mat2(1.0f);
}

Mat2 Mat2::rotate(float angle) {
	return Mat2 {
		cos(angle), -sin(angle),
		sin(angle), cos(angle)
	};
}

Mat2 Mat2::scale(const Vec2& s) {
	return Mat2{
		s.x, 0,
		0, s.y
	};
}

Mat2 Mat2::transpose(const Mat2& m) {
	return Mat2{
		m.entry[0], m.entry[2],
		m.entry[1], m.entry[3],
	};
}

Mat2 Mat2::inverse(Mat2& matrix) {
	float det = matrix.determinant();
	if (det == 0) throw DerterminatEqualsZero();
	float invDet = 1 / det;
	return invDet * Mat2{ matrix.entry[3], -matrix.entry[1], -matrix.entry[2], matrix.entry[0] };
}

float Mat2::determinant() {
	return entry[0] * entry[3] - entry[1] * entry[2];
}

Mat2 operator*(const Mat2 left, const Mat2& right) {
	float temp[4];
	for (int row = 0; row < 2; row++)
		for (int col = 0; col < 2; col++) {
			float sum = 0.0f;
			for (int e = 0; e < 2; e++) {
				sum += left.entry[e + row * 2] * right.entry[col + e * 2];
			}
			temp[col + row * 2] = sum;
		}
	return Mat2(temp);
}

Mat2 Mat2::operator*=(const Mat2& other) {
	float temp[4];
	for (int row = 0; row < 2; row++)
		for (int col = 0; col < 2; col++) {
			float sum = 0.0f;
			for (int e = 0; e < 2; e++) {
				sum += entry[e + row * 2] * other.entry[col + e * 2];
			}
			temp[col + row * 2] = sum;
		}
	for (int i = 0; i < 4; i++)
		entry[i] = temp[i];
	return *this;
}

Mat2 operator+(const Mat2 left, const Mat2& right) {
	float temp[4];
	for (int i = 0; i < 4; i++)
		temp[i] = left.entry[i] + right.entry[i];
	return Mat2(temp);
}

Mat2 Mat2::operator+=(const Mat2& other) {
	for (int i = 0; i < 4; i++)
		entry[i] += other.entry[i];
	return *this;
}

Mat2 operator-(const Mat2 left, const Mat2& right) {
	float temp[4];
	for (int i = 0; i < 4; i++)
		temp[i] = left.entry[i] - right.entry[i];
	return Mat2(temp);
}

Mat2 Mat2::operator-=(const Mat2& other) {
	for (int i = 0; i < 4; i++)
		entry[i] -= other.entry[i];
	return *this;
}

Mat2 operator*(const Mat2 left, const float scalar) {
	float temp[4];
	for (int i = 0; i < 4; i++)
		temp[i] = left.entry[i] * scalar;
	return Mat2(temp);
}

Mat2 Mat2::operator*=(const float scalar) {
	float temp[4];
	for (int i = 0; i < 4; i++)
		temp[i] = entry[i] * scalar;
	for (int i = 0; i < 4; i++)
		entry[i] = temp[i];
	return *this;
}

Vec2 operator*(const Mat2& left, const Vec2& right) {
	return Vec2(left.entry[0] * right.x + left.entry[1] * right.y,
		left.entry[2] * right.x + left.entry[3] * right.y);
}
