#include "Formula.h"
#include "parse.h"

using FormulaPtr = std::unique_ptr<Formula>;
using std::optional;
using std::pair;
using std::string;

using CellReference = pair<size_t, size_t>;

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
		size_t length = result->first + 1;
		const int x = result->second;
		if (x < 0) return {};

		str = str.substr(1 + length);
		if (str.empty() || str[0] != 'N') return {};

		if (auto result = parseInt(str.substr(1))) {
			const int y = result->second;
			if (y < 0) return {};
			length += result->first + 1;

			return {pair(length, CellReference{x, y})};
		}
	}

	return {};
}

FormulaPtr fromString(string str)
{
	if (auto result = parseInt(str); result && result->first == str.size()) {
		return std::make_unique<formulas::Atomic>(
		    formulas::Atomic{static_cast<float>(result->second)});
	}

	if (auto result = parseReference(str);
	    result && result->first == str.size()) {
		CellReference const& ref = result->second;
		return std::make_unique<formulas::Reference>(ref);
	}
}

std::optional<std::pair<size_t, FormulaPtr>>
formulas::parse(std::string const& str)
{
}
