#pragma once

#include <memory>

namespace formulas {
struct Formula
{
	virtual float compute() const = 0;
	virtual ~Formula(){};
};

struct Atomic : Formula
{
	const float value;
	float compute() const override { return value; }
	virtual ~Atomic(){};
};

struct BinaryFormula : Formula
{
	enum OP { plus, minus, multiply, divide, exp };
	const OP op;

	const std::unique_ptr<Formula> left, right;

	float compute() const override;

	virtual ~BinaryFormula(){};
};

} // namespace formulas

using Formula = formulas::Formula;
