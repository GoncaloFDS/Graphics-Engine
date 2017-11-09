#include "catch.hpp"
#include "Quat.h"


TEST_CASE("TEST 1 : Rotation of 90 degrees about the y-axis") {
	Vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
	Quat q(90.0f, axis);

	Quat vi = { 0.0f, 7.0f, 0.0f, 0.0f };

	Quat qe = { 0.0f, 0.0f, 0.0f, -7.0f };

	Quat qinv = q.Inverse();
	qinv.Clean();
	Quat qf = (q*vi) * qinv;

	CHECK(qf == qe);
}