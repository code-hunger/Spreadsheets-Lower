#include "parse.h"

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
	if (auto res = parse<IntCell>(str, cellSeparator)) return res;
	if (auto res = parse<StringCell>(str, cellSeparator)) return res;
	if (auto res = parse<EmptyCell>(str, cellSeparator)) return res;
	return parse_unquoted(str);
}
