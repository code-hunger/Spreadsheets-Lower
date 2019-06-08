#pragma once

#include <memory>

namespace formulas {
struct Context
{
	virtual float at(size_t x, size_t y) const = 0;
	virtual ~Context(){};
};

struct Formula
{
	virtual float compute(Context const&) const = 0;
	virtual ~Formula(){};
};

struct Atomic : Formula
{
	const float value;
	float compute(Context const&) const override { return value; }
	virtual ~Atomic(){};
};

struct Binary : Formula
{
	enum OP { plus, minus, multiply, divide, exp };
	const OP op;

	const std::unique_ptr<Formula> left, right;

	float compute(Context const&) const override;

	virtual ~Binary(){};
};

struct Reference : Formula
{
	size_t x, y;
	float compute(Context const& c) const override { return c.at(x, y); }
	virtual ~Reference(){};
};

} // namespace formulas

using Formula = formulas::Formula;
