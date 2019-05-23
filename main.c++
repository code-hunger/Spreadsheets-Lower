#include <iostream>

#include "Table.h"

int main(int, char* argv[])
{
	ParseResult result = parse(argv[1], ',');

	if (result.has_value()) {
		auto& value = result.value();
		auto& cell = value.second;
		std::cout << "Parsed " << cell->str() << " with " << value.first
		          << " chars." << std::endl;
	} else {
		std::cout << "Couldn't parse!" << std::endl;
	}

	Table table{std::cin};

	table.print();
}
