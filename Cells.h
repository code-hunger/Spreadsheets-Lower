#pragma once

#include "Formula.h"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <memory>
#include <optional>

using std::string;
using std::unique_ptr;

struct Cell
{
	virtual string str() const = 0;
};

struct ContextCell
{
	virtual string str(formulas::Context const&) const = 0;
};

using ParseResult = std::optional<std::pair<size_t, unique_ptr<Cell>>>;

class IntCell : public Cell, ContextCell
{
	int value;

public:
	static ParseResult parse(string str);

	IntCell(int value) : value(value) {}

	IntCell(IntCell const& other) : value(other.value) {}

	string str() const override;
	string str(formulas::Context const&) const override { return str(); };
};

struct EmptyCell : Cell, ContextCell
{
	static ParseResult parse(string)
	{
		return {std::pair{0, std::make_unique<EmptyCell>()}};
	}

	string str() const override { return ""; }
	string str(formulas::Context const&) const override { return str(); };
};

class StringCell : public Cell, ContextCell
{
	string value;

public:
	StringCell(string str) : value(str) {}

	static ParseResult parse(string);

	string str() const override { return '"' + value + '"'; }
	string str(formulas::Context const&) const override { return str(); };
};

class FormulaCell : public Cell, ContextCell
{
public:
	FormulaCell (unique_ptr<Formula> value) : value(std::move(value)) {}

	static ParseResult parse(string);

	string str() const override
	{
		std::cerr << "Computing a formula requires a context!" << std::endl;
	}
	string str(formulas::Context const& context) const override
	{
		return boost::lexical_cast<std::string>(value->compute(context));
	};

private:
	unique_ptr<Formula> value;
};
