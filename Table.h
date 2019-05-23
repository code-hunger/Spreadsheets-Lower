#pragma once

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Cells.h"
#include "parse.h"

using std::string;

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
