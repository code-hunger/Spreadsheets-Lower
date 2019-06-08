#include "Table.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

using std::vector;

vector<size_t> computeWidths(Table::DataT const&);

Table::Table(std::istream& input)
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
				std::cerr << "Couldn't parse line " << data.size() << std::endl;
				break;
			}
		}

		data.push_back(std::move(row));
	}

	columnWidthCache = computeWidths(data);
}

Table::Table(DataT&& data)
    : data(std::move(data)), columnWidthCache(computeWidths(data))
{
}

// Very ugly unicode support in C++
// Copied directly from http://www.zedwood.com/article/cpp-utf8-strlen-function
// Don't have nerves to deal with it alone
size_t utf8_strlen(const string& str)
{
	size_t q = 0;
	for (size_t i = 0, ix = str.length(); i < ix; i++, q++) {
		int c = (unsigned char)str[i];
		if (c >= 0 && c <= 127)
			i += 0;
		else if ((c & 0xE0) == 0xC0)
			i += 1;
		else if ((c & 0xF0) == 0xE0)
			i += 2;
		else if ((c & 0xF8) == 0xF0)
			i += 3;
		// else if (($c & 0xFC) == 0xF8) i+=4; // 111110bb //byte 5, unnecessary
		// in 4 byte UTF-8 else if (($c & 0xFE) == 0xFC) i+=5; // 1111110b
		// //byte 6, unnecessary in 4 byte UTF-8
		else
			return 0; // invalid utf8
	}
	return q;
}

vector<size_t> computeWidths(Table::DataT const& data)
{
	vector<size_t> widths;

	for (auto const& row : data) {
		widths.resize(std::max(widths.size(), row.size()));
		for (size_t i = 0; i < row.size(); ++i) {
			widths[i] = std::max(widths[i], utf8_strlen(row[i]->str()));
		}
	}

	return widths;
}

string pad(string str, size_t length)
{
	str.insert(str.begin(), length - utf8_strlen(str), ' ');
	return str;
}

void Table::print() const
{
	for (RowT const& row : data) {
		using std::cout;
		cout << "|";
		for (size_t i = 0; i < row.size(); ++i) {
			cout << " " << pad(row[i]->str(), columnWidthCache[i]) << " |";
		}
		cout << std::endl;
	}
}
