#pragma once
#include <exception>
#define CloseTozero 1e-5

struct DerterminatEqualsZero : std::exception {
	const char* what() const noexcept { return "Its not possible to Invert a Matrix with determinant 0\n"; }
};

struct VectorDividedByZero : std::exception {
	const char* what() const noexcept { return "Its not possible to Divided a Vector by Zero\n"; }
};

struct NormalizeZeroVetor : std::exception {
	const char* what() const noexcept { return "Its not possible to Normalize Zero Vector \n"; }
};