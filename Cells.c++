#include "Cells.h"
#include "parse.h"

ParseResult IntCell::parse(string str)
{
	if (auto result = parseInt(str))
		return {std::pair{result->first,
		                  std::make_unique<IntCell>(result->second)}};
	return {};
}

string IntCell::str() const
{
	string x;
	int value = this->value;
	while (value) {
		x += value % 10 + '0';
		value /= 10;
	}

	const size_t size = x.size();
	for (size_t i = 0; i < size / 2; ++i) {
		std::swap(x[i], x[size - i - 1]);
	}

	return x;
}

ParseResult StringCell::parse(string str)
{
	bool behindBackquote = false;
	char quote = '"';
	if (str[0] != quote) return {};

	size_t length = 1;
	string value;
	while ((behindBackquote || str[length] != quote) and length < str.size()) {

		if (!behindBackquote && str[length] == '\\')
			behindBackquote = true;
		else {
			behindBackquote = false;
			value += str[length];
		}

		++length;
	}

	if (behindBackquote || str[length] != quote) return {};

	++length; // to include the final quote

	return {std::pair(length, std::make_unique<StringCell>(value))};
}

ParseResult FormulaCell::parse(string str)
{
	if (auto formula = formulas::parse(str)) {
		return {std::pair(formula->first, std::make_unique<FormulaCell>(
		                                      std::move(formula->second)))};
	}
	return {};
}
