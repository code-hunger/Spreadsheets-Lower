#pragma once

#include <optional>
#include <memory>
#include "Formula.h"

using std::string;
using std::unique_ptr;

struct Cell
{
	virtual string str() const = 0;
};

using ParseResult = std::optional<std::pair<size_t, unique_ptr<Cell>>>;

class IntCell : public Cell
{
	int value;

public:
	static ParseResult parse(string str);

	IntCell(int value) : value(value) {}

	IntCell(IntCell const& other) : value(other.value) {}

	string str() const;
};

struct EmptyCell : Cell
{
	static ParseResult parse(string)
	{
		return {std::pair{0, std::make_unique<EmptyCell>()}};
	}

	string str() const { return ""; }
};

class StringCell : public Cell
{
	string value;

public:
	StringCell(string str) : value(str) {}

	static ParseResult parse(string);

	string str() const { return '"' + value + '"'; }
};

class FormulaCell : public Cell
{
public:
	FormulaCell (unique_ptr<Formula> value) : value(std::move(value)) {}

	static ParseResult parse(string);

	string str() const { throw "Computing a formula requires a context!"; }

private:
	unique_ptr<Formula> value;
};
