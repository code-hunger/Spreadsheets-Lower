#include "Cells.h"

ParseResult IntCell::parse(string str)
{
	if (str.empty()) return {};

	bool is_signed = str[0] == '-' || str[0] == '+', positive = str[0] != '-';
	size_t chars_read = 0;

	if (is_signed) {
		str = str.substr(1);
		++chars_read;
	}

	int num = 0;

	for (char c : str) {
		if (!isdigit(c)) break;

		num = num * 10 + (c - '0');
		++chars_read;
	}

	if ((is_signed && chars_read == 1) or (!is_signed && chars_read == 0))
		return {};

	if (!positive) num *= -1;

	return {std::pair{chars_read, std::make_unique<IntCell>(num)}};
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
