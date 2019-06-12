#include "Formula.h"
#include "parse.h"

#include <iostream>

using FormulaPtr = std::unique_ptr<Formula>;
using std::optional;
using std::pair;
using std::string;

using CellReference = pair<size_t, size_t>;

bool isOperator(char c)
{
	for (char op : formulas::operators) {
		if (c == op) return true;
	}
	return false;
}

formulas::Binary::OP formulas::Binary::toOp(char op)
{
	switch (op) {
	case '+':
		return plus;
	case '-':
		return minus;
	case '*':
		return multiply;
	case '/':
		return divide;
	case '^':
		return exp;
	}

	std::cerr << "Unknown operator " << op << std::endl;
	throw "Unknown operator";
}

// Please loook at how the Reference and the Number parser are implemented in
// Perl6:
/*
 * my regex float { \-? \d* \.? \d+ }
 * multi fromString (Str $str where /^ <float> $/) { return $str.Rat }
 * multi fromString (Str $str where /^R(\N)C(\N)$/) { return ($0 - 1, $1 - 1) }
 * */

optional<pair<size_t, CellReference>> parseReference(string str)
{
	if (str.empty() || str[0] != 'R') return {};

	if (auto result = parseInt(str.substr(1))) {
		size_t length = result->first + 1; // including the 'R'
		const int x = result->second;
		if (x < 0) return {};

		str = str.substr(length);
		if (str.empty() || str[0] != 'C') return {};

		if (auto result = parseInt(str.substr(1))) {
			const int y = result->second;
			if (y < 0) return {};
			length += result->first + 1;

			return {pair(length, CellReference{x, y})};
		}
	}

	return {};
}

size_t readTerm(string const& str)
{
	size_t depth = 0;
	bool hasBraces = !str.empty() && str.front() == '(';

	size_t i = 0;
	for (; i < str.size(); ++i) {
		const char c = str[i];
		if (c == '(') {
			++depth;
			if (!hasBraces) return i;
		}
		if (c == ')') {
			if (depth == 0 || !hasBraces) return i;
			if (depth == 1) return i + 1;
			--depth;
		}
		if (depth == 0 && isOperator(c)) return i;
	}

	if (depth != 0) {
		std::cerr << "Unballanced braces in '" << str << "'" << std::endl;
		throw "X";
	}

	return i;
}

FormulaPtr fromString(string);

FormulaPtr fromTermAndRest(string left, string rest)
{
	if (rest.empty()) {
		boost::trim(left);
		return fromString(left);
	}
	if (left.empty()) {
		std::cerr << "Left term empty at " << rest << std::endl;
		throw "Left term empty error.";
	}

	boost::trim_left(rest);
	char op = rest[0];

	if (!isOperator(op)) {
		std::cerr << "Expected operator, found " << op << " after " << left
		          << std::endl;
	}

	boost::trim(left);
	rest = rest.substr(1);
	boost::trim(rest);
	return std::make_unique<formulas::Binary>(fromString(left), op,
	                                          fromString(rest));
}

FormulaPtr fromString(string str)
{
	std::cout << "From string: '" << str << "'" << std::endl;
	if (auto result = parseInt(str); result && result->first == str.size()) {
		return std::make_unique<formulas::Atomic>(
		    formulas::Atomic{static_cast<float>(result->second)});
	}

	if (auto result = parseReference(str);
	    result && result->first == str.size()) {
		CellReference const& ref = result->second;
		return std::make_unique<formulas::Reference>(ref);
	}

	size_t termLength = readTerm(str);
	string term = str.substr(0, termLength - 1), rest = str.substr(termLength);

	boost::trim(term);
	boost::trim(rest);

	if (!term.empty() && term[0] == '(' && term.back() == ')')
		term = term.substr(1, term.size() - 1);

	return fromTermAndRest(term, rest);
}

std::optional<FormulaPtr> formulas::parse(std::string str)
{
	std::cout << "WILL parse formula '" << str << "'" << std::endl;
	boost::trim(str);
	try {
		return fromString(str);
	} catch (...) {
		std::cerr << "Error building formula." << std::endl;
		return {};
	}
}
