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

	virtual ~Cell(){};
};

struct ContextCell
{
	virtual string str(formulas::Context const&) const = 0;
	virtual float asFloat(formulas::Context const&) const = 0;

	virtual ~ContextCell(){};
};

using ParseResult = std::optional<std::pair<size_t, unique_ptr<ContextCell>>>;

class IntCell : public Cell, public ContextCell
{
	int value;

public:
	static ParseResult parse(string str);

	IntCell(int value) : value(value) {}

	IntCell(IntCell const& other) : value(other.value) {}

	string str() const override;
	string str(formulas::Context const&) const override { return str(); };
	float asFloat(formulas::Context const&) const override { return value; }

	virtual ~IntCell(){};
};

class FloatCell : public Cell, public ContextCell
{
	float value;

public:
	static ParseResult parse(string str);

	FloatCell(float value) : value(value) {}

	FloatCell(FloatCell const& other) : value(other.value) {}

	string str() const override;
	string str(formulas::Context const&) const override { return str(); };
	float asFloat(formulas::Context const&) const override { return value; }

	virtual ~FloatCell(){};
};

struct EmptyCell : Cell, public ContextCell
{
	static ParseResult parse(string)
	{
		return {std::pair{0, std::make_unique<EmptyCell>()}};
	}

	string str() const override { return ""; }
	string str(formulas::Context const&) const override { return str(); };
	float asFloat(formulas::Context const&) const override { return 0; }

	virtual ~EmptyCell(){};
};

class StringCell : public Cell, public ContextCell
{
	string value;

public:
	StringCell(string str) : value(str) {}

	static ParseResult parse(string);

	string str() const override { return '"' + value + '"'; }
	string str(formulas::Context const&) const override { return str(); };

	float asFloat(formulas::Context const&) const override;

	virtual ~StringCell(){};
};

class FormulaCell : public Cell, public ContextCell
{
public:
	FormulaCell(unique_ptr<Formula> value) : value(std::move(value)) {}

	static ParseResult parse(string);

	string str() const override
	{
		std::cerr << "Computing a formula requires a context!" << std::endl;
		return "[ ?? ]";
	}
	string str(formulas::Context const& context) const override
	{
		return boost::lexical_cast<std::string>(value->compute(context));
	};

	float asFloat(formulas::Context const&) const override;

	~FormulaCell() {}

private:
	unique_ptr<Formula> value;
};
