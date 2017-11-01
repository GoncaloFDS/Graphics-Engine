#include "catch.hpp"
#include "Vec3.h"

TEST_CASE("Vec3 Cunstructor", "[Vec3]") {
	Vec3 v(1, 2, 3);
	CHECK(v.x == 1);
	CHECK(v.y == 2);
	CHECK(v.z == 3);
}

TEST_CASE("Vec3 Comparator", "[Vec3]") {
	Vec3 a(1, 2, 3);
	Vec3 b(1, 2, 3);
	Vec3 c(0, 0, 0);
	CHECK(a == b);
	CHECK_FALSE(a == c);
	CHECK(a != c);
	CHECK_FALSE(a != b);
}

TEST_CASE("Vec3 Arithmatic Operations", "[Vec3]") {
	Vec3 a(1, 2, 3);
	Vec3 b(4, 5, 6);

	SECTION("Sum") {
		CHECK(a + b == Vec3(5, 7, 9));
	}

	SECTION("Sum Shorthand") {
		a += b;
		CHECK(a == Vec3(5, 7, 9));
	}

	SECTION("Subtraction") {
		CHECK(a - b == Vec3(-3, -3, -3));
	}

	SECTION("Subtraction Shorthand") {
		a -= b;
		CHECK(a == Vec3(-3, -3, -3));
	}

	SECTION("Multiplication") {
		CHECK(a * 5 == Vec3(5, 10, 15));
		CHECK(5 * a == Vec3(5, 10, 15));
	}

	SECTION("Multiplication Shorthand") {
		a *= 5;
		CHECK(a == Vec3(5, 10, 15));
	}

	SECTION("Division") {
		CHECK(a / 5 == Vec3(1.f / 5, 2.f / 5, 3.f/5));
	}

	SECTION("Division Shorthand") {
		a /= 5;
		CHECK(a == Vec3(1.f / 5, 2.f / 5, 3.f/5));
	}

}

TEST_CASE("Vec3 Methods", "[Vec3]") {
	Vec3 a(1, 2, 3);
	Vec3 b(4, 5, 6);
	SECTION("Quadrance") {
		CHECK(a.Quadrance() == 14.f);
	}

	SECTION("Magnitude") {
		Vec3 magV(1, 2, 3);
		CHECK(magV.Magnitude() == (float) sqrt(14.f));
	}

	SECTION("Normalize") {
		Vec3 na(5, 0, 0);
		na.Normalize();
		CHECK(na == Vec3(1, 0, 0));

		Vec3 nb(0, 5, 0);
		nb.Normalize();
		CHECK(nb == Vec3(0, 1, 0));
		
		Vec3 nc(0, 0, 5);
		nc.Normalize();
		CHECK(nc == Vec3(0, 0, 1));

		Vec3 v2(1, 1, 1);
		v2.Normalize();
		CHECK(v2 == Vec3( (float)(1 / sqrt(3)),
			(float)(1 / sqrt(3)),
			(float)(1 / sqrt(3))));
	}

	SECTION("Dot product") {
		CHECK(a.Dot(b) == 32.f);
		CHECK(a == Vec3(1, 2, 3));
		CHECK(b == Vec3(4, 5, 6));
	}

	SECTION("Cross product") {
		CHECK(a.Cross(b) == Vec3(-3, 6, -3));
		CHECK(a == Vec3(1, 2, 3));
		CHECK(b == Vec3(4, 5, 6));
	}
}

TEST_CASE("Vec3 operation properties", "[Vec3]") {
	Vec3 a(1, 2, 3);
	Vec3 b(4, 5, 6);
	Vec3 c(7, 6, 8);
	float k = 5.f;
	float p = 2.f;
	SECTION("Sum Commutative Property") {
		CHECK(a + b == b + a);
	}

	SECTION("Sum Associative  Property") {
		CHECK(a + (b + c) == (a + b) + c);
	}

	SECTION("Sum Identity Property") {
		CHECK(a + Vec3(0, 0, 0) == a);
	}

	SECTION("Sum null Property") {
		CHECK(a - a == Vec3(0, 0, 0));
	}

	SECTION("Distributive Property 1") {
		CHECK(k * (a + b) == k*a + k*b);
	}

	SECTION("Distributive Property 2") {
		CHECK((k + p)*a == k*a + p*a);
	}

	SECTION("Multiplication Commutative Property") {
		CHECK((k*p)*a == k*(p*a));
	}

	SECTION("Multiplication Identity Property") {
		CHECK(1 * a == a);
	}

	SECTION("Dot Product Commutative Property 1") {
		CHECK(a.Dot(b) == b.Dot(a));
	}

	SECTION("Dot Product Distributive Property") {
		CHECK(a.Dot(b + c) == a.Dot(b) + a.Dot(c));
	}

	SECTION("Dot Product Commutative Property 2") {
		CHECK((k*a).Dot(b) == k*(a.Dot(b)));
	}

	SECTION("Dot Product null Property") {
		CHECK(Vec3(0, 0, 0).Dot(a) == 0);
	}

	SECTION("Cross Product Perpendicular Property") {
		Vec3 t1(2, 0, 0);
		Vec3 t2(0, 3, 0);
		Vec3 t3(0, 0, 4);
		CHECK(t1.Cross(t2).Normalize() == t3.Normalize());
		CHECK(t2.Cross(t3).Normalize() == t1.Normalize());
		CHECK(t3.Cross(t1).Normalize() == t2.Normalize());
	}

	SECTION("Cross Product == 0") {
		Vec3 t1(2, 0, 0);
		Vec3 t2(5, 0, 0);
		CHECK(t1.Cross(t2) == Vec3(0, 0, 0));
		CHECK(t2.Cross(t1) == Vec3(0, 0, 0));
		CHECK(t1.Cross(Vec3(0, 0, 0)) == Vec3(0, 0, 0));
		CHECK(Vec3(0,0,0).Cross(t1) == Vec3(0, 0, 0));
	}

	SECTION("Cross Product Inverse") {
		CHECK(a.Cross(b) == -1*b.Cross(a));
	}

	SECTION("Cross Product Scalar Associtivity") {
		CHECK((k*a).Cross(b) == a.Cross(k*b));
		CHECK((k*a).Cross(b) == k*(a.Cross(b)));
	}

	SECTION("Cross Product and Sum Distributive Property") {
		CHECK(a.Cross(b +c) == a.Cross(b) + a.Cross(c));
	}

	SECTION("Cross Product and Dot Distributive Property") {
		CHECK(a.Dot(b.Cross(c)) == (a.Cross(b).Dot(c)));
	}

	SECTION("Cross Product vector triple product") {
		CHECK(a.Cross(b.Cross(c)) == (c.Dot(a))*b - (a.Dot(b))*c);
	}

}