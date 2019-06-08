#pragma once

#include <boost/algorithm/string.hpp>
#include <string>

#include "Cells.h"

using std::string;

ParseResult parse_unquoted(string str);

bool is_prefix(string pref, string str);

inline bool is_prefix(char prefix, string str)
{
	return str.size() >= 1 && str[0] == prefix;
}

template <typename CellT> ParseResult parse(string str, string cellSeparator)
{
	ParseResult result = CellT::parse(str);

	if (!result) return result;
	auto& value = result.value();
	string remainingString = (str.substr(value.first));
	boost::trim(remainingString);

	// A match is only successful at the end of a string or at the separator
	if (remainingString.empty() || is_prefix(cellSeparator, remainingString))
		return result;

	return {};
}

ParseResult parse(string str, string cellSeparator);

inline ParseResult parse(string str, char cellSeparator = ',')
{
	return parse(str, string{cellSeparator});
}

std::optional<std::pair<size_t, int>> parseInt(string);
