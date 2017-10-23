#include "Mat3.h"
#include "MathExceptions.h"

Mat3::Mat3(float e0, float e1, float e2, 
	float e3, float e4, float e5, 
	float e6, float e7, float e8) 
	: entry {
	e0, e1, e2,
	e3,	e4, e5,
	e6, e7,	e8 }
{}

Mat3::Mat3(const float e[9]) {
	for (int i = 0; i < 9; i++)
		entry[i] = e[i];
}

Mat3::Mat3(const float diagonal) {
	entry[0] = diagonal;
	entry[4] = diagonal;
	entry[8] = diagonal;
}

Mat3::Mat3(const Mat4& m) 
	: entry {
		m.entry[0], m.entry[1], m.entry[2],
		m.entry[4], m.entry[5], m.entry[6],
		m.entry[8], m.entry[9], m.entry[10]
	}
{}

bool Mat3::operator==(Mat3& other) const {
	for (int i = 0; i < 9; i++)
		if (fabs(entry[i] - other.entry[i]) > CloseTozero)
			return false;
	return true;
}

bool Mat3::operator!=(Mat3& other) const {
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Mat3& m) {
	for(int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			std::cout << m.entry[i * 3 + j] << "\t";
		std::cout << std::endl;
	}
		
	return os;
}

void Mat3::Clean() {
	for (int i = 0; i < 9; i++)
		if (fabs(entry[i]) < CloseTozero)
			entry[i] = 0.f;
}

float Mat3::Get(int i, int j) {
	return entry[i * 3 + j];
}

float Mat3::Determinant() {
	return entry[0] * entry[4] * entry[8]
		+ entry[3] * entry[7] * entry[2]
		+ entry[6] * entry[1] * entry[5]
		- entry[6] * entry[4] * entry[2]
		- entry[7] * entry[5] * entry[0]
		- entry[1] * entry[3] * entry[8];
}

Mat3 operator*(const Mat3 left, const Mat3& right) {
	float temp[9];
	for (int row = 0; row < 3; row++)
		for (int col = 0; col < 3; col++) {
			float sum = 0.0f;
			for (int e = 0; e < 3; e++) {
				sum += left.entry[e + row * 3] * right.entry[col + e * 3];
			}
			temp[col + row * 3] = sum;
		}
	return Mat3(temp);
}

Mat3 Mat3::operator*=(const Mat3& other) {
	float temp[9];
	for (int row = 0; row < 3; row++)
		for (int col = 0; col < 3; col++) {
			float sum = 0.0f;
			for (int e = 0; e < 3; e++) {
				sum += entry[e + row * 3] * other.entry[col + e * 3];
			}
			temp[col + row * 3] = sum;
		}
	for (int i = 0; i < 9; i++)
		entry[i] = temp[i];
	return *this;
}

Mat3 operator+(const Mat3 left, const Mat3& right) {
	float temp[9];
	for (int i = 0; i < 9; i++)
		temp[i] = left.entry[i] + right.entry[i];
	return Mat3(temp);
}

Mat3 Mat3::operator+=(const Mat3& other) {
	for (int i = 0; i < 9; i++)
		entry[i] += other.entry[i];
	return *this;
}

Mat3 operator-(const Mat3 left, const Mat3& right) {
	float temp[9];
	for (int i = 0; i < 9; i++)
		temp[i] = left.entry[i] - right.entry[i];
	return Mat3(temp);
}

Mat3 operator*(const Mat3 left, const float scalar) {
	float temp[9];
	for (int i = 0; i < 9; i++)
		temp[i] = left.entry[i] * scalar;
	return Mat3(temp);
}

Mat3 Mat3::operator*=(const float scalar) {
	for (int i = 0; i < 9; i++)
		entry[i] *= scalar;
	return *this;
}

Mat3 Mat3::operator-=(const Mat3& other) {
	for (int i = 0; i < 9; i++)
		entry[i] -= other.entry[i];
	return *this;
}

Vec3 operator*(const Mat3& left, const Vec3& right) {
	return Vec3(left.entry[0] * right.x + left.entry[1] * right.y + left.entry[2] * right.z,
		left.entry[3] * right.x + left.entry[4] * right.y + left.entry[5] * right.z,
		left.entry[6] * right.x + left.entry[7] * right.y + left.entry[8] * right.z);
}
