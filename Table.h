#pragma once

#include <memory>
#include <vector>

#include "Cells.h"
#include "parse.h"

class Table
{
	using RowT = std::vector<std::unique_ptr<Cell>>;
	std::vector<RowT> data;

public:
	Table() = default;

	Table(std::istream&);

	void print();
};
