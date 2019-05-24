#pragma once

#include <memory>
#include <vector>

#include "Cells.h"
#include "parse.h"

struct Table
{
	using RowT = std::vector<std::unique_ptr<Cell>>;
	using DataT = std::vector<RowT>;

	Table() = default;
	Table(DataT&& data) : data(std::move(data)) {}
	Table(std::istream&);

	void print();

private:
	DataT data;

	std::vector<size_t> columnWidthCache;
};
