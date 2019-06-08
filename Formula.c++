#include "Formula.h"

using namespace formulas;

float compute(float left, Binary::OP op, float right)
{
	switch (op) {
	case Binary::plus:
		return left + right;

	case Binary::minus:
		return left - right;

	default:
		throw "Formula operation unsupported.";
	}
}

float Binary::compute() const {
	return ::compute(left->compute(), op, right->compute());
}
