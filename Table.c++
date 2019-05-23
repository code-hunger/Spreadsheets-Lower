#include "Table.h"
#include <iostream>

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
}

void Table::print()
{
	for (RowT const& row : data) {
		for (auto& cell : row) {
			std::cout << cell->str() << " | ";
		}
		std::cout << std::endl;
	}
}
