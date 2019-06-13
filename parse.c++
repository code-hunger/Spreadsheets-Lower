#include "parse.h"
#include <cmath>

ParseResult parse_unquoted(string str)
{
	size_t length = 0;

	for (size_t i = 0; i < str.size() && str[i] != ','; ++i)
		++length;

	return {
	    std::pair{length, std::make_unique<StringCell>(str.substr(0, length))}};
}

bool is_prefix(string pref, string str)
{
	for (size_t i = 0; i < pref.size(); ++i) {
		if (str[i] != pref[i] || i >= str.size()) return false;
	}
	return true;
}

ParseResult parse(string str, string cellSeparator)
{
	if (auto res = parse<FloatCell>(str, cellSeparator)) return res;
	if (auto res = parse<IntCell>(str, cellSeparator)) return res;
	if (auto res = parse<StringCell>(str, cellSeparator)) return res;
	if (auto res = parse<EmptyCell>(str, cellSeparator)) return res;
	if (auto res = parse<FormulaCell>(str, cellSeparator)) return res;
	return parse_unquoted(str);
}

std::optional<std::pair<size_t, int>> parseInt(string str)
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

	return {std::pair(chars_read, num)};
}

std::optional<std::pair<size_t, float>> parseFloat(string str)
{
	const auto a = parseInt(str);
	if (!a) return {};

	const int whole = a->second;

	str = str.substr(a->first);

	if (str[0] != '.') return {};

	size_t chars_read = a->first + 1;

	const auto b = parseInt(str.substr(1));
	if (!b) return {};

	const int afterPoint = b->second;
	chars_read += b->first;

	const float num = whole + afterPoint / std::pow(10, (b->first));
	return {std::pair(chars_read, num)};
}
