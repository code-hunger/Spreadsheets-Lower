#pragma once

#include <memory>
#include <vector>

#include "Cells.h"
#include "parse.h"

struct Table
{
	using RowT = std::vector<std::unique_ptr<ContextCell>>;
	using DataT = std::vector<RowT>;

	Table() = default;
	Table(DataT&& data);
	Table(std::istream&);

	void print() const;

	struct ContextAccessor;

private:
	DataT data;

	std::vector<size_t> columnWidthCache;
};
