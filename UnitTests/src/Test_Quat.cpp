#include "catch.hpp"
#include "Quat.h"

const float PI = static_cast<float>(3.14159265359);


TEST_CASE("TEST 1 : Rotation of 90 degrees about the y-axis") {
	Vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
	Quat q(PI/2, axis);

	Quat vi = { 0.0f, 7.0f, 0.0f, 0.0f };

	Quat qe = { 0.0f, 0.0f, 0.0f, -7.0f };

	Quat qinv = q.inverse();
	qinv.clean();
	Quat qf = (q*vi) * qinv;

	CHECK(qf == qe);
}