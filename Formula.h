#pragma once

#include <memory>
#include <optional>
#include <string>

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
	Atomic(float value) : value(value) {}
	float compute(Context const&) const override { return value; }
	virtual ~Atomic(){};
};

struct Binary : Formula
{
	enum OP { plus, minus, multiply, divide, exp };
	const OP op;

	const std::unique_ptr<Formula> left, right;

	Binary(std::unique_ptr<Formula> left, OP op, std::unique_ptr<Formula> right)
	    : op(op), left(std::move(left)), right(std::move(right))
	{
	}

	float compute(Context const&) const override;

	virtual ~Binary(){};
};

struct Reference : Formula
{
	size_t x, y;
	float compute(Context const& c) const override { return c.at(x, y); }
	Reference(std::pair<size_t, size_t> cell) : x(cell.first), y(cell.second) {}
	virtual ~Reference(){};
};

std::optional<std::pair<size_t, std::unique_ptr<Formula>>>
parse(std::string const& str);
} // namespace formulas

using Formula = formulas::Formula;
