#include "catch.hpp"
#include "../Engine/Mat3.h"
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
	SECTION("Identity") {
		Mat3 i{
			1, 0, 0,
			0, 1, 0,
			0, 0, 1 };
		CHECK(i == Mat3::Identity());
	}
	SECTION("Rotate") {
		Mat3 rot{
			0, 0, 1,
			1, 0, 0,
			0, 1, 0 };
		Vec3 v{ 1, 1, 1 };
		Mat3 m = Mat3::Rotate(2.f * PI / 3.f, v);
		CHECK(rot == m);

	}

	SECTION("Scale") {
		Mat3 i{
			2, 0, 0,
			0, 3, 0,
			0, 0, 1 };
		Mat3 s = Mat3::Scale(Vec3{ 2, 3, 1 });
		CHECK(i == s);
	}

	SECTION("Translation") {
		Mat3 i{
			1, 0, 1,
			0, 1, 2,
			0, 0, 1 };
		Mat3 s = Mat3::Translation(Vec2{ 1, 2});
		CHECK(i == s);
	}

	SECTION("Inverse") {
		Mat3 i{
			1, 2, 3,
			3, 2, 1,
			2, 1, 3 };
		Mat3 result {
			-5, 3, 4,
			7, 3, -8,
			1, -3, 4
		};
		Mat3 s = Mat3::Inverse(i);
		Mat3 a = Mat3::Inverse(s);
		CHECK(i == a);
		result *= 1.f / 12.f;
		CHECK(s == result);

		Mat3 t{
			1, 0, 1,
			0, 1, 3,
			0, 0, 1 };

		Mat3 t_inv{
			1, 0, -1,
			0, 1, -3,
			0, 0, 1 };

		CHECK(t_inv == Mat3::Inverse(t));
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