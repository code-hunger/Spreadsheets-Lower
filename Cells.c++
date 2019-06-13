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
	int value = this->value >= 0 ? this->value : - this->value;
	while (value) {
		x += value % 10 + '0';
		value /= 10;
	}
	if(this->value < 0) x += '-';

	const size_t size = x.size();
	for (size_t i = 0; i < size / 2; ++i) {
		std::swap(x[i], x[size - i - 1]);
	}

	return x;
}

ParseResult FloatCell::parse(string str) {
	if (auto result = parseFloat(str))
		return {std::pair{result->first,
		                  std::make_unique<FloatCell>(result->second)}};
	return {};
}

string FloatCell::str() const
{
	return boost::lexical_cast<string>(value);
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
	if (!is_prefix("=[", str)) return {};

	for (size_t i = 2; i < str.size(); ++i) {
		if (str[i] == '[') return {};

		if (str[i] == ']') {
			if (auto formula = formulas::parse(str.substr(2, i - 2))) {
				return {std::pair(i + 1, std::make_unique<FormulaCell>(
				                             std::move(formula.value())))};
			}
			return {};
		}
	}

	return {};
}
