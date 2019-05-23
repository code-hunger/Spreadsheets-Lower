#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Cells.h"

using std::string;

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

bool is_prefix(char prefix, string str)
{
	return str.size() >= 1 && str[0] == prefix;
}

template<typename CellT>
ParseResult parse(string str, string cellSeparator)
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

ParseResult parse(string str, string cellSeparator)
{
	if (auto res = parse<IntCell>(str, cellSeparator)) return res;
	if (auto res = parse<StringCell>(str, cellSeparator)) return res;
	if (auto res = parse<EmptyCell>(str, cellSeparator)) return res;
	return parse_unquoted(str);
}

ParseResult parse(string str, char cellSeparator = ',')
{
	return parse(str, string{cellSeparator});
}

using std::vector;

class Table
{
	using RowT = vector<unique_ptr<Cell>>;
	vector<RowT> data;

public:
	Table() = default;

	Table(std::istream& input)
	{
		while (!input.eof()) {
			RowT row;
			string buffer;

			std::getline(input, buffer);

			boost::trim(buffer);

			while (buffer.size() > 0) {
				boost::trim(buffer);

				if (ParseResult result = parse(buffer)) {
					row.push_back(std::move(result->second));
					buffer = buffer.substr(result->first);
					boost::trim_left(buffer);
					if (buffer.size() > 0 && buffer[0] == ',')
						buffer = buffer.substr(1);
				} else {
					std::cout << "Couldn't parse line " << data.size() << std::endl;
					break;
				}
			}

			data.push_back(std::move(row));
		}
	}

	void print()
	{
		for (RowT const& row : data) {
			for (auto& cell : row) {
				std::cout << cell->str() << " | ";
			}
			std::cout << std::endl;
		}
	}
};
