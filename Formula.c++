#include "Formula.h"

using namespace formulas;

float Binary::compute() const {
	switch (op) {
	case Binary::plus:
		return left->compute() + right->compute();

	case Binary::minus:
		return left->compute() - right->compute();

	default:
		throw "Formula operation unsupported.";
	}
}
