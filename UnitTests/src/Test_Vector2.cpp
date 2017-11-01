#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Vec2.h"


TEST_CASE("Vec2 Cunstructor", "[Vec2]") {
	Vec2 v(1, 2);
	CHECK(v.x == 1);
	CHECK(v.y == 2);
}

TEST_CASE("Vec2 Comparator", "[Vec2]") {
	Vec2 a(1, 2);
	Vec2 b(1, 2);
	Vec2 c(0, 0);
	CHECK(a == b);
	CHECK_FALSE(a == c);
	
	CHECK(a != c);
	CHECK_FALSE(a != b);
}

TEST_CASE("Vec2 Arithmatic Operations", "[Vec2]") {
	Vec2 a(1, 2);
	Vec2 b(3, 4);
	
	SECTION("Sum") {
		CHECK(a + b == Vec2(4, 6));
	}

	SECTION("Sum Shorthand") {
		a += b;
		CHECK(a == Vec2(4, 6));
	}
	
	SECTION("Subtraction") {
		CHECK(a - b == Vec2(-2, -2));
	}

	SECTION("Subtraction Shorthand") {
		a -= b;
		CHECK(a == Vec2(-2, -2));
	}
	
	SECTION("Multiplication") {
		CHECK(a * 5 == Vec2(5, 10));
		CHECK(5 * a == Vec2(5, 10));
	}

	SECTION("Multiplication Shorthand") {
		a *= 5;
		CHECK(a == Vec2(5, 10));
	}

	SECTION("Division") {
		CHECK(a / 5 == Vec2(1.f/5, 2.f/5));
	}

	SECTION("Division Shorthand") {
		a /= 5;
		CHECK(a == Vec2(1.f / 5, 2.f / 5));
	}

}

TEST_CASE("Vec2 Methods", "[Vec2]") {
	Vec2 a(1, 2);
	Vec2 b(3, 4);
	SECTION("Quadrance") {
		CHECK(a.Quadrance() == 5.f);
	}

	SECTION("Magnitude") {
		Vec2 magV(6, 8);
		CHECK(magV.Magnitude() == 10);
	}

	SECTION("Normalize") {
		Vec2 na(5, 0);
		na.Normalize();
		CHECK(na.x == 1);
		CHECK(na.y == 0);

		Vec2 nb(0, 5);
		nb.Normalize();
		CHECK(nb.x == 0);
		CHECK(nb.y == 1);

		Vec2 v2(1, 1);
		v2.Normalize();
		CHECK(v2.x == (float)(1 / sqrt(2)));
		CHECK(v2.y == (float)(1 / sqrt(2)));
	}
	SECTION("Dot product") {
		CHECK(a.Dot(b) == 11.f);
		CHECK(a == Vec2(1, 2));
		CHECK(b == Vec2(3, 4));
	}
}

TEST_CASE("Vec2 operation properties", "[Vec2]") {
	Vec2 a(1, 2);
	Vec2 b(3, 4);
	Vec2 c(5, 6);
	float k = 5.f;
	float p = 2.f;
	SECTION("Sum Commutative Property") {
		CHECK(a + b == b + a);
	}

	SECTION("Sum Associative  Property") {
		CHECK(a + (b + c) == (a + b) + c);
	}

	SECTION("Sum Identity Property") {
		CHECK(a + Vec2(0, 0) == a);
	}

	SECTION("Sum null Property") {
		CHECK(a - a == Vec2(0, 0));
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
		CHECK(Vec2(0,0).Dot(a) == 0);
	}

}