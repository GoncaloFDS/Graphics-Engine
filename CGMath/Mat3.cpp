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

Mat3 Mat3::Identity() {
	return Mat3(1.f);
}

Mat3 Mat3::Rotate(const float angle, Vec3& axis){
	//Rodrigues
	axis.Normalize();
	return Mat3::Identity() + sin(angle) * CrossMatrix(axis)
		+ (1 - cos(angle)) * SqrCrossMatrix(axis);
}

Mat3 Mat3::Scale(const Vec3& s) {
	return Mat3 {
		s.x, 0, 0,
		0, s.y, 0,
		0, 0, s.z
	};
}

Mat3 Mat3::Translation(const Vec2& t) {
	return Mat3{
		1, 0, t.x,
		0, 1, t.y,
		0, 0, 1
	};
}

Mat3 Mat3::Transpose(const Mat3& m) {
	return Mat3 {
		m.entry[0], m.entry[3], m.entry[6],
		m.entry[1], m.entry[4], m.entry[7],
		m.entry[2], m.entry[5], m.entry[8],
	};
}

Mat3 Mat3::Inverse(Mat3& m) {
	float det = m.Determinant();
	if (det == 0) throw DerterminatEqualsZero();
	float invDet = 1 / det;
	float result[9];
	result[0] = (m.Get(1, 1) * m.Get(2, 2) - m.Get(2, 1) * m.Get(1, 2)) * invDet;
	result[3] = (m.Get(1, 2) * m.Get(2, 0) - m.Get(1, 0) * m.Get(2, 2)) * invDet;
	result[6] = (m.Get(1, 0) * m.Get(2, 1) - m.Get(2, 0) * m.Get(1, 1)) * invDet;
	result[1] = (m.Get(0, 2) * m.Get(2, 1) - m.Get(0, 1) * m.Get(2, 2)) * invDet;
	result[4] = (m.Get(0, 0) * m.Get(2, 2) - m.Get(0, 2) * m.Get(2, 0)) * invDet;
	result[7] = (m.Get(2, 0) * m.Get(0, 1) - m.Get(0, 0) * m.Get(2, 1)) * invDet;
	result[2] = (m.Get(0, 1) * m.Get(1, 2) - m.Get(0, 2) * m.Get(1, 1)) * invDet;
	result[5] = (m.Get(1, 0) * m.Get(0, 2) - m.Get(0, 0) * m.Get(1, 2)) * invDet;
	result[8] = (m.Get(0, 0) * m.Get(1, 1) - m.Get(1, 0) * m.Get(0, 1)) * invDet;
	return Mat3(result);
}

Mat3 Mat3::CrossMatrix(const Vec3& k) {
	return Mat3 {
		0, -k.z, k.y,
		k.z, 0, -k.x,
		-k.y, k.x, 0
	};
}

Mat3 Mat3::SqrCrossMatrix(const Vec3& k) {
	return Mat3{
		-(k.y * k.y) -(k.z*k.z), k.x*k.y, k.x*k.z,
		k.x*k.y, -(k.x * k.x) - (k.y*k.y), k.y*k.z,
		k.x*k.z, k.y*k.z, -(k.x * k.x) - (k.y*k.y)
	};
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
