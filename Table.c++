#include "Table.h"
#include <algorithm>
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

vector<size_t> computeWidths(Table::DataT const& data)
{
	vector<size_t> widths;

	for (auto const& row : data) {
		widths.resize(row.size());
		for (size_t i = 0; i < row.size(); ++i) {
			widths[i] = std::max(widths[i], row[i]->str().size());
		}
	}

	return widths;
}

void Table::print() const
{
	for (RowT const& row : data) {
		for (auto& cell : row) {
			std::cout << cell->str() << " | ";
		}
		std::cout << std::endl;
	}
}
