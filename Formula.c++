#include "Formula.h"

#include <cmath>

using namespace formulas;

float compute(float left, Binary::OP op, float right)
{
	switch (op) {
	case Binary::plus:
		return left + right;

	case Binary::minus:
		return left - right;

	case Binary::multiply:
		return left * right;

	case Binary::divide:
		return left / right;

	case Binary::exp:
		return std::pow(left, right);

	default:
		throw "Formula operation unsupported.";
	}
}

float Binary::compute(Context const& c) const
{
	return ::compute(left->compute(c), op, right->compute(c));
}
