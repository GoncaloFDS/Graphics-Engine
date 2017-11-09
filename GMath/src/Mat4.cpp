#include "Mat4.h"
#include "MathExceptions.h"
#include "Mat3.h"
#include "MatrixFactory.h"


/* OpenGl Column major format
 * 0 4 8  12
 * 1 5 9  13
 * 2 6 10 14
 * 3 7 11 15
 */

Mat4::Mat4(float e0, float e1, float e2, float e3, float e4, float e5, float e6, float e7, float e8, float e9,
	float e10, float e11, float e12, float e13, float e14, float e15) 
	: entry {
	e0, e1, e2, e3,
	e4, e5, e6, e7,
	e8, e9, e10, e11,
	e12, e13, e14, e15 }
{}

Mat4::Mat4(const float diagonal) {
	entry[0] = diagonal;
	entry[5] = diagonal;
	entry[10] = diagonal;
	entry[15] = diagonal;
}

Mat4::Mat4(const Mat3& m) 
	: entry {
		m.entry[0], m.entry[1] , m.entry[2] , 0,
		m.entry[3], m.entry[4] , m.entry[5] , 0,
		m.entry[6], m.entry[7] , m.entry[8] , 0,
		0, 0, 0, 1
	}
{}

Mat4::Mat4(const float e[16]) {
	for (int i = 0; i < 16; i++)
		entry[i] = e[i];
}

float Mat4::Get(int i, int j) {
	return entry[i * 4 + j];
}

std::ostream& operator<<(std::ostream& os, const Mat4& m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			os<< m.entry[i * 4 + j] << "\t\t";
		os << std::endl;
	}
	return os;
}

bool Mat4::operator==(Mat4& other) const {
	for (int i = 0; i < 16; i++)
		if (fabs(entry[i] - other.entry[i]) > CloseTozero)
			return false;
	return true;
}

bool Mat4::operator!=(Mat4& other) const {
	return !(*this == other);
}

void Mat4::Clean() {
	for (int i = 0; i < 16; i++)
		if (fabs(entry[i]) < CloseTozero)
			entry[i] = 0.f;
}

Mat4 operator*(const Mat4 left, const Mat4& right) {
	float temp[16];
	for(int row = 0; row < 4; row++)
		for(int col = 0; col < 4; col++) {
			float sum = 0.0f;
			for(int e = 0; e< 4;e++) {
				sum += left.entry[e + row * 4] * right.entry[col + e * 4];
			}
			temp[col + row * 4] = sum;
		}
	return Mat4(temp);
}

Mat4& Mat4::operator*=(const Mat4& other) {
	float temp[16];
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++) {
			float sum = 0.0f;
			for (int e = 0; e < 4; e++) {
				sum += entry[e + row * 4] * other.entry[col + e * 4];
			}
			temp[col + row * 4] = sum;
		}
	for (int i = 0; i < 16; i++)
		entry[i] = temp[i];
	return *this;
}

Mat4 operator+(const Mat4 left, const Mat4& right) {
	float temp[16];
	for (int i = 0; i < 16; i++)
		temp[i] = left.entry[i] + right.entry[i];
	return Mat4(temp);
}

Mat4& Mat4::operator+=(const Mat4& other) {
	for (int i = 0; i < 16; i++)
		entry[i] += other.entry[i];
	return *this;
}

Mat4 operator-(const Mat4 left, const Mat4& right) {
	float temp[16];
	for (int i = 0; i < 16; i++)
		temp[i] = left.entry[i] - right.entry[i];
	return Mat4(temp);
}

Mat4& Mat4::operator-=(const Mat4& other) {
	for (int i = 0; i < 16; i++)
		entry[i] -= other.entry[i];
	return *this;
}

Vec3 operator*(const Mat4& left, const Vec3& right) {
	return Vec3(left.entry[0] * right.x + left.entry[1] * right.y + left.entry[2] * right.z + left.entry[3],
		left.entry[4] * right.x + left.entry[5] * right.y + left.entry[6] * right.z + left.entry[7],
		left.entry[8] * right.x + left.entry[9] * right.y + left.entry[10] * right.z + left.entry[11]);
}

Mat4 operator*(const Mat4 left, const float scalar) {
	float temp[16];
	for (int i = 0; i < 16; i++)
		temp[i] = left.entry[i] * scalar;
	return Mat4(temp);
}


float Mat4::Determinant() {
	float temp[16];

	temp[0] = entry[5] * entry[10] * entry[15] -
		entry[5] * entry[11] * entry[14] -
		entry[9] * entry[6] * entry[15] +
		entry[9] * entry[7] * entry[14] +
		entry[13] * entry[6] * entry[11] -
		entry[13] * entry[7] * entry[10];

	temp[4] = -entry[4] * entry[10] * entry[15] +
		entry[4] * entry[11] * entry[14] +
		entry[8] * entry[6] * entry[15] -
		entry[8] * entry[7] * entry[14] -
		entry[12] * entry[6] * entry[11] +
		entry[12] * entry[7] * entry[10];

	temp[8] = entry[4] * entry[9] * entry[15] -
		entry[4] * entry[11] * entry[13] -
		entry[8] * entry[5] * entry[15] +
		entry[8] * entry[7] * entry[13] +
		entry[12] * entry[5] * entry[11] -
		entry[12] * entry[7] * entry[9];

	temp[12] = -entry[4] * entry[9] * entry[14] +
		entry[4] * entry[10] * entry[13] +
		entry[8] * entry[5] * entry[14] -
		entry[8] * entry[6] * entry[13] -
		entry[12] * entry[5] * entry[10] +
		entry[12] * entry[6] * entry[9];

	const float determinant = entry[0] * temp[0] + entry[1] * temp[4] + entry[2] * temp[8] + entry[3] * temp[12];
	
	if (determinant == 0)
		throw DerterminatEqualsZero();

	return 1.0f / determinant;
}

Mat4 Mat4::operator*=(const float scalar) {
	float temp[16];
	for (int i = 0; i < 16; i++)
		temp[i] = entry[i] * scalar;
	for (int i = 0; i < 16; i++)
		entry[i] = temp[i];
	return *this;
}

Mat4::~Mat4() = default;

Mat4 Mat4::RowMajor() {
	return MatrixFactory::Transpose(Mat4(*this));
}

