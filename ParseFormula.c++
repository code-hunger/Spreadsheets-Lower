#include "Formula.h"
#include "parse.h"

using FormulaPtr = std::unique_ptr<Formula>;
using std::string;

FormulaPtr fromString(string str)
{
	if (auto result = parseInt(str); result && result->first == str.size()) {
		return std::make_unique<formulas::Atomic>(
		    formulas::Atomic{static_cast<float>(result->second)});
	}
}

std::optional<std::pair<size_t, FormulaPtr>>
formulas::parse(std::string const& str)
{
}
