#include "catch.hpp"
#include "Mat3.h"
#include "MatrixFactory.h"
const float PI = 3.1415926535897;

TEST_CASE("Matrix Constructor") {
	Mat3 m{0, 1, 2, 3, 4, 5, 6, 7, 8};
	for (int i = 0; i < 9; i++)
		CHECK(i == m.entry[i]);

}

TEST_CASE("Matrix Comparator") {
	Mat3 b{0, 1, 2, 3, 4, 5, 6, 7, 8};
	Mat3 a{0, 1, 2, 3, 4, 5, 6, 7, 8};
	Mat3 c{0, 1, 3, 3, 4, 5, 6, 7, 8};
	CHECK(a == b);
	CHECK_FALSE(a != b);
	CHECK(a != c);
	CHECK_FALSE(a == c);

}

TEST_CASE("Matrix Factory") {
	
	SECTION("Rotate") {
		Mat3 rot{
			0, 0, 1,
			1, 0, 0,
			0, 1, 0 };
		Vec3 v{ 1, 1, 1 };
		Mat3 m = MatrixFactory::Rotate(2.f * PI / 3.f, v);
		CHECK(rot == m);

	}

	SECTION("Scale") {
		Mat3 i{
			2, 0, 0,
			0, 3, 0,
			0, 0, 1 };
		Mat3 s = MatrixFactory::Scale(Vec3{ 2, 3, 1 });
		CHECK(i == s);
	}

}

TEST_CASE("Determinant") {
	Mat3 i{
		1, 2, 3,
		3, 2, 1,
		2, 1, 3 
	};
	CHECK(i.Determinant() == -12);

}

TEST_CASE("Matrix Arithmatic") {

	SECTION("Matrix Multiplication") {
		Mat3 a{ 
			0, 1, 0,
			0, 0, 1,
			1, 0, 0
		};
		Mat3 b{ 
			0, 0, 1,
			1, 0, 0,
			0, 1, 0
		};
		Mat3 c{ 
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};

		CHECK(a * b == c);
		a *= b;
		CHECK(a == c);
	}

	SECTION("Matrix Sum") {
		Mat3 a{
			0, 1, 0,
			0, 0, 1,
			1, 0, 0
		};
		Mat3 b{
			0, 0, 1,
			1, 0, 0,
			0, 1, 0
		};
		Mat3 c{
			0, 1, 1,
			1, 0, 1,
			1, 1, 0
		};

		CHECK(a + b == c);
		a += b;
		CHECK(a == c);
	}

	SECTION("Matrix Subtraction") {
		Mat3 a{
			0, 1, 0,
			0, 0, 1,
			1, 0, 0
		};
		Mat3 b{
			0, 0, 1,
			1, 0, 0,
			0, 1, 0
		};
		Mat3 c{
			0, 1, -1,
			-1, 0, 1,
			1, -1, 0
		};

		CHECK(a - b == c);
		a -= b;
		CHECK(a == c);
	}

}